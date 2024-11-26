#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

// Class representing a single node (cell) in the grid
class Node {
public:
    Node(int x, int y) : x(x), y(y), g(0), h(0), parent(nullptr) {}

    int getF() const { return g + h; } // Calculate the total cost (F = G + H)

    int x, y; // Node position
    int g, h; // g: cost from start, h: heuristic to goal
    Node* parent; // Pointer to the parent node for path reconstruction
};

// Class implementing the A* pathfinding algorithm
class AStar {
public:
    AStar(int width, int height, Node* start, Node* goal, const std::vector<std::vector<int>>& obstacles)
        : width(width), height(height), start(start), goal(goal), obstacles(obstacles) {}

    // Main function to find the shortest path
    std::vector<Node*> findPath() {
        // Priority queue for open list using a lambda to compare nodes by their F values
        std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>> openList(
            [](Node* a, Node* b) { return a->getF() > b->getF(); });

        openList.push(start); // Add the start node to the open list
        std::vector<Node*> closedList; // List of visited nodes

        // Main A* loop
        while (!openList.empty()) {
            Node* current = openList.top(); // Get the node with the lowest F value
            openList.pop();

            // If the goal is reached, reconstruct the path
            if (current->x == goal->x && current->y == goal->y) {
                return reconstructPath(current);
            }

            closedList.push_back(current); // Mark the current node as visited

            // Explore neighbors (up, down, left, right)
            for (const auto& direction : directions) {
                int newX = current->x + direction[0];
                int newY = current->y + direction[1];

                // Skip nodes that are out of bounds or blocked by obstacles
                if (newX < 0 || newY < 0 || newX >= width || newY >= height || obstacles[newY][newX] == 1)
                    continue;

                Node* neighbor = new Node(newX, newY);

                // Skip nodes already in the closed list
                if (std::find_if(closedList.begin(), closedList.end(), 
                                 [&](Node* n) { return n->x == neighbor->x && n->y == neighbor->y; }) != closedList.end())
                    continue;

                // Calculate costs and set the parent
                neighbor->g = current->g + 1;
                neighbor->h = heuristic(neighbor);
                neighbor->parent = current;

                openList.push(neighbor); // Add neighbor to the open list
            }
        }

        return {}; // Return empty if no path is found
    }

private:
    int width, height; // Grid dimensions
    Node* start; // Start node
    Node* goal; // Goal node
    const std::vector<std::vector<int>>& obstacles; // Obstacle grid

    // Directions for neighbors: up, down, left, right
    const std::vector<std::vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Heuristic function (Manhattan distance)
    int heuristic(Node* node) {
        return std::abs(node->x - goal->x) + std::abs(node->y - goal->y);
    }

    // Function to reconstruct the path by backtracking from the goal node
    std::vector<Node*> reconstructPath(Node* current) {
        std::vector<Node*> path;
        while (current != nullptr) {
            path.push_back(current);
            current = current->parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
};

// Function to generate random obstacles on the grid
void generateObstacles(std::vector<std::vector<int>>& grid, int numObstacles, int size, Node* start, Node* goal) {
    int width = grid[0].size();
    int height = grid.size();
    std::srand(std::time(0)); // Seed for randomness

    for (int i = 0; i < numObstacles; ++i) {
        while (true) {
            int x = std::rand() % (width - size); // Random top-left corner
            int y = std::rand() % (height - size);

            // Check if the obstacle overlaps with the start or goal
            bool overlapsStartOrGoal = false;
            for (int dx = 0; dx < size; ++dx) {
                for (int dy = 0; dy < size; ++dy) {
                    if ((x + dx == start->x && y + dy == start->y) || 
                        (x + dx == goal->x && y + dy == goal->y)) {
                        overlapsStartOrGoal = true;
                        break;
                    }
                }
                if (overlapsStartOrGoal) break;
            }

            // Place the obstacle if there's no overlap
            if (!overlapsStartOrGoal) {
                for (int dx = 0; dx < size; ++dx) {
                    for (int dy = 0; dy < size; ++dy) {
                        grid[y + dy][x + dx] = 1; // Mark the cell as an obstacle
                    }
                }
                break;
            }
        }
    }
}

int main() {
    int width = 100, height = 100; // Grid size
    Node* start = new Node(0, 0); // Start point
    Node* goal = new Node(99, 99); // End point

    // Initialize the grid and generate obstacles
    std::vector<std::vector<int>> obstacles(height, std::vector<int>(width, 0));
    generateObstacles(obstacles, 50, 8, start, goal);

    // Create A* object and find the path
    AStar astar(width, height, start, goal, obstacles);
    std::vector<Node*> path = astar.findPath();

    // Create the SFML window for visualization
    sf::RenderWindow window(sf::VideoMode(800, 800), "A* Pathfinding with Obstacles");
    float cellSize = 800.0f / width; // Size of each grid cell

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw the grid
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(j * cellSize, i * cellSize);

                // Color based on content
                if (obstacles[i][j] == 1) {
                    cell.setFillColor(sf::Color::Red); // Obstacle
                } else {
                    cell.setFillColor(sf::Color::White); // Empty space
                }

                cell.setOutlineThickness(-1);
                cell.setOutlineColor(sf::Color::Black);
                window.draw(cell);
            }
        }

        // Highlight the path
        for (Node* node : path) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(node->x * cellSize, node->y * cellSize);
            cell.setFillColor(sf::Color::Yellow); // Path
            window.draw(cell);
        }

        // Highlight start and goal points
        sf::RectangleShape startCell(sf::Vector2f(cellSize, cellSize));
        startCell.setPosition(start->x * cellSize, start->y * cellSize);
        startCell.setFillColor(sf::Color::Green); // Start point
        window.draw(startCell);

        sf::RectangleShape goalCell(sf::Vector2f(cellSize, cellSize));
        goalCell.setPosition(goal->x * cellSize, goal->y * cellSize);
        goalCell.setFillColor(sf::Color::Blue); // Goal point
        window.draw(goalCell);

        window.display();
    }

    return 0;
}
