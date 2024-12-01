# A* Pathplanning Visualization

This repository demonstrates a visualization of the A* pathfinding algorithm in a 2D grid using **C++** and the **SFML** graphics library. The program generates random obstacles, highlights the start and end points, and animates the pathfinding process.

<p align="center">
  <img src="astar_visualization.png" alt="A* Pathplanning Visualization" width="500"/>
</p>

## Features
- Randomly generated obstacles displayed in red.
- Start point (green), end point (blue), and the computed path (yellow).
- Visualization of the grid with horizontal and vertical lines.

## Requirements
- **C++ compiler** supporting C++17 or later.
- **SFML** (Simple and Fast Multimedia Library) for rendering.

## Installation

### Install SFML on Ubuntu
1. Update your system:

   $ sudo apt update
   
2. Install SFML:

   $ sudo apt install libsfml-dev

### Compile the Code

Use a C++ compiler to compile the program:

  $ g++ -std=c++2a -o astar_visualization astar_visualization.cpp -lsfml-graphics -lsfml-window -lsfml-system

### Run the Program

Run the compiled program:

  $ ./astar_visualization

## How It Works
1. The program initializes a 100x100 grid.
2. Obstacles are generated randomly, ensuring they do not overlap the start or end points.
3. The A* algorithm computes the shortest path from the top-left corner (start) to the bottom-right corner (goal).
4. The visualization shows the grid, obstacles, and the computed path.
