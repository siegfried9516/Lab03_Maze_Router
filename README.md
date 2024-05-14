---
# MR (Maze_Router)

## Introduction
A Program implements a 2D maze router to complete two terminal routing promblems.
We use A* algorithm to find the shortest path between two terminals.

## Features
- Initaliztion grid map with blocks and nets
- Implementation of the A* algorithm for finding optimal paths
- Support for routing multiple network connections in 2D plane(two terminals)
- Functions for printing paths and outputting results

## Dependencies
- C++11 standard library
- Head files: `<fstream>`, `<sstream>`, `<queue>`, `<algorithm>`, `<cmath>`, `<vector>`, `<list>`, `<iostream>`

## Usage
1. Compile the code: ```$ make```
2. Run the program: input_file template is in `input` folder, output_file can customize directory and name.
```$./lab3 [input_file] [output_file]```
3. View the result: Remenber chmod 755 draw to run
```$ ./input/draw [input_file] [output_file]```
it will generate a SVG file for visualization, which can be opened with internet browser.

## File Structure
- `MR.h`: Defined the `MR` class, which contains the main functions for routing.
- `MR.cpp`: Implements various functions within the `MR` class.
- `block.h`, `net.h`, `node.h`: Define classes for blocks, nets, and nodes respectively.

## Function List
- `MR(std::ifstream& input_file)`: Read the input file and initialize num of path and the routing order.
- `void MR::init_map()`: Initialize thei grid map with blocks and pins (wire cannot go through the source or target pins or the block).
- `int MR::routing()`: main function for routing,if the defalut routing order is not suitable, it will rip-up all connections and update it be the first routing order, until all connections are routed.
- `bool MR::astar()`: 1. . 2. Add the source node to the open list. 3. While the open list is not empty, pop the node with the lowest f-score from the open list. 4. If the popped node is the target node, return true. 5. For each neighbor of the popped node, calculate the tentative g-score and f-score. 6. If the neighbor is not in the closed list and the tentative g-score is lower than the current g-score of the neighbor in the open list, update the neighbor's parent node, g-score, and f-score. 7. Add the neighbor to the open list. 8. Mark the popped node as closed. 9. Return false if the target node is not found.
1. **Initialize**: Initialize two containers, ``Open`` and ``Closed``, These are going to store the nodes that we need to check out(``Open``) and the nodes we've already visited (``Closed``). Then it unlocks the start and end nodes of the net we're routing.
2. **Start and Goal Setup**: start node(source_x,source_y) and goal node(target_x,target_y), then we calcular the G value for start node (should be 0),and update the h value for start node by ``int MR::heuristic(int s_x,int s_y,int t_x,int t_y)`` which is the Manhattan distance of current node to goal node.
3. **Open list and Closed list setup**: Add the start node to ``Open`` and use Heap to sort ``Open`` based on f value(f = g + h), and update the Heap every time we push back a node or pop back a node from ``Open``. ``Closed`` is a set to store the nodes we've already visited.
4. **Main looping**: As long as there are nodes in the ``Open``, it keep looping. It takes the node with the lowest f value from ``Open`` (Open.front()), here we call it ``tmp``.
5. **Check if the popped node is the goal**: if ``tmp`` is the goal, then we start backtracking from the goal node to the start node, and record the path in a list. Also set up the path grid ``is_visited()`` the stop others net crossing or reaching the same grid.
6. **Finding the neighbors of ``tmp``**: if ``tmp`` isn't the goal, it's time to explore its neighbors.For each neighbor of ``tmp``, if it isn't in ``Closed`` and it's not a block, then we check if it's in ``Open`` or not. If it's not in ``Open``, we will update its g-value and h-value and update its parent node be ``tmp``,and push it into ``Open`` and update the Heap. If it's in ``Open``, we will check if the g-value of the neighbor which parent is ``tmp`` is lover than the current g-value of the neighbor in ``Open``, if true, we will update the neighbor's parent node and the g-value.
7. **Return value**: If we find the path to goal, return true, else the ``Open`` is empty, return false.
## Authors
This code was developed by Sieg Lau. For any questions or suggestions, you can report an issue.
---