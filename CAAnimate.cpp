#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "Empirical/include/emp/math/Random.hpp"

emp::web::Document doc{"target"};

// force refresh is "shift" then hit the refresh button
emp::Random random_generator(7); // random seed, can change in future

class CAAnimator : public emp::web::Animate {

// grid width and height
const int num_h_boxes = 25;
const int num_w_boxes = 25;
const double RECT_SIDE = 25;
const double width{num_w_boxes * RECT_SIDE};
const double height{num_h_boxes * RECT_SIDE};


//some vectors to hold information about the CA
std::vector<std::vector<float>> cells;
std::vector<std::vector<float>> next_cells;
        

// where we'll draw
emp::web::Canvas canvas{width, height, "canvas"};

public:

    CAAnimator() {
        // shove canvas into the div
        // along with some control buttons
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");
        
        // fill cells with 0 to start 
        cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0.0));
        next_cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0.0));

        // fill cells with random numbers after
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                cells[x][y] = random_generator.GetDouble(); // between 0 and 1
            }
        }
    }

    float getNeighborAverage(int x, int y) {
        float neighbor_sum = 0.0;
        int neighbor_count = 0;
        
        // Check all 8 neighbors around, including the edges
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; // Skip the cell itself
                
                // If on the edge, this wraps around to the other side
                int nx = (x + dx + num_w_boxes) % num_w_boxes;
                int ny = (y + dy + num_h_boxes) % num_h_boxes;
                
                neighbor_sum += cells[nx][ny];
                neighbor_count++;
            }
        }
        // Count should always be 8 in theory
        return neighbor_sum / neighbor_count;
    }

    void DoFrame() override {
        canvas.Clear();

        // Draw the cells accordingly
        for (int x = 0; x < num_w_boxes; x++){
             for (int y = 0; y < num_h_boxes; y++) {
                // H=120 is green, S=1 for full saturation, and V varies with cell value
                canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, 
                    emp::ColorHSV(120, 1.0, cells[x][y]), "black");
            }
        }

        // Update the cells based on CA rules
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                float neighborAverage = getNeighborAverage(x, y);
                float currentState = cells[x][y];

                // General outline of rules
                // 1. If neighbor average is higher than current cell, cell grows faster
                // 2. If cell reaches threshold, it resets
                //    (like dying from too much competition)
                // 3. All cells have a constant baseline growth

                // Base growth rate that applies to all cells
                float baseGrowthRate = 0.02;
                
                // Additional growth rate when neighbor average is higher
                float adaptiveGrowthRate = 0.05;
                
                // Reset threshold: when cell value reaches this, it drops
                float resetThreshold = 0.8;
                
                // Reset value: what value cells drop to when reset
                float resetValue = 0.1;
                
                if (currentState >= resetThreshold) {
                    // Reset cell if it's reached threshold
                    next_cells[x][y] = resetValue;
                } else {
                    // Start with base growth for all cells
                    float growthAmount = baseGrowthRate;
                    
                    // Add extra growth if neighbors are higher
                    if (neighborAverage > currentState) {
                        growthAmount += adaptiveGrowthRate * (neighborAverage - currentState);
                    }
                    
                    // Apply growth
                    next_cells[x][y] = currentState + growthAmount;
                }
                
                // Add some noise so it is more random
                float noise = 0.02 * (random_generator.GetDouble() - 0.5);
                next_cells[x][y] += noise;
                
                
                // Ensure values stay between 0 and 1
                if (next_cells[x][y] > 1.0) next_cells[x][y] = 1.0;
                if (next_cells[x][y] < 0.0) next_cells[x][y] = 0.0;
            }
        }
        // Swap the cell vectors to be able to continue the simulation
        cells.swap(next_cells);
    }

};

CAAnimator animator;

int main() {
    //Have animator call DoFrame once to start
    animator.Step();
}