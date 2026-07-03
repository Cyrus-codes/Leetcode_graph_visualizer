# LeetCode Graph Testcase Visualizer

A modular, dependency-free developer tool designed to automatically parse LeetCode graph testcase strings and render them in the browser using an interactive, physics-driven 2D Canvas layout. 

This project provides a clean bridge between abstract representation strings (adjacency lists, adjacency matrices, and edge lists) and visual understanding, making it easy to analyze and debug graph algorithms.

---

## 🚀 Key Features

* **Auto-Detection Engine**: The core parsing engine automatically detects the input format (Edge List, Adjacency List, or Adjacency Matrix) based on its structure and nesting.
* **Interactive Canvas**: Drag nodes to adjust layout dynamically. Nodes behave like physical entities under forces of gravity, edge tension (attraction), and node distance (repulsion).
* **Dependency-Free Architecture**: No external packages or framework installations are required. The backend uses Python's standard HTTP libraries, and the frontend is written in vanilla HTML5/CSS3/JavaScript.
* **C++ Backend Parsing**: Leverage high-performance C++17 string tokenization for parsing nested arrays and constructing the structural graph representation.

---

## 📁 Project Structure

```text
Leetcode_graph_visualizer/
├── Makefile                # GNU make compilation script
├── b.ps1                   # Windows PowerShell compilation script
├── s.py                    # Python HTTP server (Static file server & API endpoint)
├── bin/                    # Contains compiled executables (Git-ignored)
│   └── a[.exe]             # Compiled C++ parser executable
├── graphcpp/
│   └── a.cpp               # C++ graph parser & format detection source
└── visualizer/
    └── a.html              # Frontend single-page app (UI + physics canvas)
```

---

## ⚙️ How It Works (Data Flow)

1. **User Action**: The user inputs a LeetCode testcase string (e.g. `[[2,1,1],[2,3,4],[3,4,1]]`) into the visualizer Web UI and clicks **Parse**.
2. **API Request**: The webpage sends a POST request with the string to the Python backend endpoint (`/api/parse`).
3. **C++ Execution**: The Python server invokes the compiled C++ parser (`bin/a`) in a subprocess, feeding it the input.
4. **Format Detection & Assembly**:
   - The C++ parser tokenizes the bracket structure.
   - It identifies if the input is an **Edge List** (inner lists of size 2/3), an **Adjacency Matrix** (square matrix), or an **Adjacency List** (default fallback).
   - The parser constructs the nodes and edges, checks for validation errors, and prints a structured JSON payload to standard output.
5. **Interactive Render**: The Python server captures the JSON and returns it to the browser. The frontend loads the graph into a force-directed spring layout simulation on a 2D Canvas.

---

## 💻 Setup & Run Instructions

### Prerequisites
* A C++ compiler supporting C++17 (`g++` is recommended).
* Python 3.6 or later.
* A modern web browser.

### 1. Compile the Parser
Run the build script corresponding to your operating system to compile the C++ parser:

**Linux / macOS:**
```bash
make
```

**Windows (PowerShell):**
```powershell
./b.ps1
```

### 2. Start the Server
Start the local HTTP server:
```bash
python s.py
```

### 3. Open the UI
Open your browser and navigate to:
👉 **[http://localhost:8000](http://localhost:8000)**

Choose a preset example or paste your own LeetCode test case to begin visualizing.
