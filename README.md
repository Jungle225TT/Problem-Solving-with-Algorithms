  

---

# Problem-Solving-with-Algorithms: Maze Generation and Resolution  

## 📋 Overview  
This project focuses on solving the classic maze generation and resolution problem. It implements algorithms to:  
1. Generate a solvable maze with a unique exit.  
2. Resolve the maze by finding the shortest path from the entrance to the exit.  

---

## 🚀 Getting Started  

### Prerequisites  
- A C compiler (e.g., GCC).  
- Basic familiarity with the command line.  

### Installation and Execution  
1. **Navigate to the project directory**:  
   ```bash
   cd Problem-Solving-with-Algorithms/session1
   ```

2. **Build the project**:  
   Use the provided Makefile to compile the source code.  
   ```bash
   make
   ```

3. **Run the maze program**:  
   Execute the maze program:  
   ```bash
   ./Maze_final
   ```

4. **Run tests**:  
   To verify the correctness of the algorithms:  
   ```bash
   make test
   ```

---

## 📂 File Structure  
```
Problem-Solving-with-Algorithms/
│
├── README.md                      # Documentation for the entire project  
├── session1/                      # Project implementation  
│   ├── Maze_final.c               # Main source code for maze generation and resolution  
│   ├── Makefile                   # Build and test automation  
│   ├── tests.c                    # Unit tests for the maze program  
│   ├── Final Report_EN.md         # Detailed final report (English)  
│   ├── report/                    # Additional report materials  
│   ├── WhatsApp Image ... .jpeg   # Relevant images and screenshots  
│   └── 截屏2024-11-16 00.18.14.png # Screenshot of output  
│
└── other sessions/                # Placeholder for other problem-solving tasks (if applicable)
```

---

## 🛠️ Features  

1. **Maze Generation**:  
   - Recursive subdivision ensures unique paths and structural randomness.  
   - Generates a maze with a single exit at the southeast corner.  

2. **Maze Resolution**:  
   - Bidirectional BFS efficiently finds the shortest path from entrance to exit.  

3. **Testing**:  
   - Includes automated test cases to validate maze correctness and pathfinding accuracy.  

---

## 📈 Algorithm Details  

### Maze Generation  
- **Recursive Division**: Divides the maze into sub-regions and connects them via single doors to ensure solvability.  

### Maze Resolution  
- **Bidirectional BFS**:  
   - Reduces search space by starting exploration from both entrance and exit.  
   - Paths merge when the meeting point is identified, confirming the shortest route.  

---

## 📝 Final Report  
For detailed insights into the algorithms, design choices, and performance evaluations, refer to:  
- `session1/Final Report_EN.md`.  

---

## 🧪 Testing  
The project includes a set of test cases (`tests.c`) to validate:  
- Correct maze structure.  
- Pathfinding accuracy and efficiency.  

---

## 👨‍💻 Authors and Acknowledgements  
This project is a collaborative effort by the team. Special thanks to **Florent Bouchez-Tichadou** for inspiring the problem statement and guidance.  

---

## ⚖️ License  
This project is licensed under the [MIT License](LICENSE).  

