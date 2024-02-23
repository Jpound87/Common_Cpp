// Path_Utilities.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

// clustered A (recursive?)* -reduce map to clusters, do A* on this low res map, then in parallel do A* on lower level maps (processed first come first serve to the higer map that needs them)

// 'uncertanties' as the quanta elements in the game -> randomness that hits every transaction and also simulated external nandom events like AI transactions outside of observation

// AI should use combvinations of path finding types and then be pushed to sligtly converge on the k clustered middle path

template <typename T, size_t N> 
struct GridNode_nD 
{
    std::vector<T> Location; // Coordinates of the node
    double g, h, f;           // Cost values for the node (g: cost from start, h: heuristic cost, f: total cost)

    GridNode_nD(std::vector<T> coords, double_t g, double_t h) : Location(coords), g(g), h(h)
    {
        f = g + h;
    }

    // Define comparison operator for priority queue
    bool operator>(const GridNode_nD& other) const
    {
        return f > other.f;
    }
};

// Function to check if a given set of coordinates is within the grid
template <typename T, size_t N>
bool isValid(const std::vector<T>& coords, const std::vector<size_t>& dimensions) 
{
    for (size_t i = 0; i < N; ++i) 
    {
        if (coords[i] < 0 || coords[i] >= dimensions[i]) 
        {
            return false;
        }
    }
    return true;
}

// A* algorithm implementation for N-dimensional grid
template <typename T, size_t N>
std::vector<std::vector<T>> aStar_nD(const std::vector<std::vector<T>>& grid,
    const std::vector<T>& start,
    const std::vector<T>& end,
    const T step) 
{
    size_t dimensions[N];

    for (size_t i = 0; i < N; ++i) 
    {
        dimensions[i] = grid.size();
    }

    // Possible movement directions (along each dimension and diagonals)
    std::vector<T> deltas;
    for (T i = -step; i <= step; i+=step) 
    {
        deltas.push_back(i);
    }

    // Initialize priority queue for open list
    std::priority_queue<GridNode_nD<T, N>, std::vector<GridNode_nD<T, N>>, std::greater<GridNode_nD<T, N>>> openList;

    // Create a vector to store the visited status of each cell
    std::vector<std::vector<bool>> visited(dimensions[0], std::vector<bool>(dimensions[1], false));

    // Start node
    GridNode_nD<T, N> startNode(start, 0, 0);

    // Enqueue the start node
    openList.push(startNode);

    while (!openList.empty()) 
    {
        // Get the node with the lowest total cost from the priority queue
        GridNode_nD<T, N> currentNode = openList.top();
        openList.pop();

        // Mark the current node as visited
        visited[currentNode.Location[0]][currentNode.Location[1]] = true;

        // Check if the current node is the goal
        if (currentNode.Location == end) 
        {
            // Reconstruct the path from end to start
            std::vector<std::vector<T>> path;
            while (currentNode.Location != start) 
            {
                path.push_back(currentNode.Location);
                T nextCoord{};
                for (size_t i = 0; i < N; ++i) 
                {
                    nextCoord[i] = currentNode.Location[i] - deltas[grid[currentNode.Location[0]][currentNode.Location[1]]];
                }
                currentNode.Location = nextCoord;
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors
        for (const T& delta : deltas) 
        {
            std::vector<T> neighborCoords = currentNode.Location;
            neighborCoords[0] += delta;

            // Check if the neighbor is within the grid and is not visited
            if (isValid<T, N>(neighborCoords, dimensions) && !visited[neighborCoords[0]][neighborCoords[1]])   
            {
                // Calculate the cost to reach the neighbor
                double_t newG = currentNode.g + step;

                // Create the neighbor node
                GridNode_nD<T, N> neighbor(neighborCoords, newG, 0);

                // Check if the neighbor is not in the open list or has a lower cost
                if (!visited[neighborCoords[0]][neighborCoords[1]]) {
                    // Calculate the heuristic cost (Manhattan distance)
                    neighbor.h = std::abs(neighborCoords[0] - end[0]) + std::abs(neighborCoords[1] - end[1]);

                    // Enqueue the neighbor
                    openList.push(neighbor);

                    // Mark the neighbor as visited
                    visited[neighborCoords[0]][neighborCoords[1]] = true;
                }
            }
        }
    }

    // If the open list is empty and the goal is not reached, return an empty path
    return {};
}

int main() 
{
    // Example usage for a 3x3 grid
    std::vector<std::vector<int>> grid = 
    {
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 0}
    };

    std::vector<int> start = { 0, 0 };
    std::vector<int> end = { 2, 2 };

    std::vector<std::vector<int>> path = aStar_nD<int, 2>(grid, start, end, 1);

    // Print the path
    for (const auto& point : path) {
        std::cout << "(" << point[0] << ", " << point[1] << ") ";
    }

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
