[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/StQAS8iq)

# Cyclical Growth Continuous Cellular Automaton

## Overview

This project demonstrates a continuous cellular automaton that creates ever-changing patterns through a "grow and reset" mechanism. Unlike Conway's Game of Life which uses binary states (alive/dead), this implementation uses continuous values between 0 and 1 for each cell, visualized as varying shades of green.

Each cell has an individual current value which is randomly generated at the start. All cells have the same base growth rate. If the average value of the surrounding cells are higher than the value of the cell itself, then it gets an elevated growth rate.

At each step, this growth rate is applied to the cell. Once the cell surpasses a threshold in its value, then it "dies" from too much competition and its value is dropped down very low again, then it restarts the cycle. 

This project was inspired by continuous versions of Conway's Game of Life, specifically with the rings and other patterns ballooning out, which is why this has a ruleset focused on cyclic behavior.