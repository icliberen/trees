# Trees and Heap

This repository brings together several C++ implementations of fundamental tree-based and heap-based data structures, each featuring insertion, deletion, and visualization.

## Contents

1. **Binary Search Tree (BST)**
   - Insertion, deletion, and level-order (BFS) traversal  
   - ASCII visualization of the tree structure  
   - Finds and displays the maximum element and its level  

2. **Balanced Binary Tree**
   - Inserts nodes while maintaining balance  
   - Removes the most recently inserted node  
   - Visualizes the tree and checks whether it is full, complete, or perfect  

3. **Heap**
   - Implements both **Min-Heap** and **Max-Heap**  
   - ASCII-art visualization using `/` and `\` for parent-child edges  
   - Switch between Min-Heap and Max-Heap modes dynamically  

4. **Red-Black Tree**
   - Interactive console program (`rbtree.cpp` / `rbtree.exe`)  
   - Supported directives:  
     - `insert <key>` / `delete <key>`  
     - `print-rbt` (prints name & color, indented with `. ` per depth)  
     - `dfs` (assigns and prints discovery/finish times)  
     - `query <d> <f>` (find node by DFS timestamps)  
     - `quit`  
   - Uses DFS timestamps (`d` = discovery, `f` = finish) for advanced queries  

## Features

- **Insertion & Deletion** across all structures  
- **Visualization**  
  - Top-down ASCII display for trees and heaps  
- **Heap Mode Switching**  
  - Toggle between Min-Heap and Max-Heap at runtime  
- **Tree Property Analysis**  
  - Full, complete, and perfect checks for binary trees  
- **DFS Timestamps**  
  - Record entry/exit times for depth-first traversals in the Red-Black Tree  

## Prerequisites

- **Windows**: Visual Studio 2019 (or later) or Embarcadero Dev C++ 
- **Cross-Platform**: g++ (C++11 or later)  

## Building & Running

- **Using Command-Line (g++)**
  - git clone https://github.com/icliberen/trees-and-heap.git
- **Compile**
  - cd trees-and-heap
  - g++ -std=c++11 -o bst          bst/main.cpp
  - g++ -std=c++11 -o balanced     balanced/main.cpp
  - g++ -std=c++11 -o heap         heap/main.cpp
  - g++ -std=c++11 -o rbtree.exe   red-black-tree/rbtree.cpp
- **Run**
  - ./bst
  - ./balanced
  - ./heap
  - ./rbtree.exe input.txt

### Using Visual Studio

1. **Clone**  
   ```bash
   git clone https://github.com/icliberen/trees-and-heap.git

