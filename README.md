SkyNet Air Traffic Control Simulator ✈️
<img width="736" height="835" alt="image" src="https://github.com/user-attachments/assets/5aef3185-393f-4b28-9aa6-630d5d66d2f1" />

A console-based Air Traffic Control (ATC) simulator built in C++ for the Data Structures semester project.
SkyNet manages aircraft in a virtual airspace using manually implemented core data structures (no STL containers for primary logic), and visualizes the airspace as a radar grid in the console.

🚀 Features
Airspace & Radar (Module A)
20x20 radar grid displaying:

A – Airports

W – Waypoints

P – Planes

. – Empty sky

Airports and waypoints stored as nodes in a weighted directed graph.

Dijkstra’s algorithm to compute the safest/shortest route from a plane’s current node to the nearest airport in case of emergency.

Plane movement between nodes updates both:

Its position in the graph

Its position on the radar grid

Landing Priority System (Module B)
Array-based Min-Heap (no std::priority_queue) to manage landing order.

Priority levels:

1 – CRITICAL (medical, engine failure, hijack)

2 – HIGH (low fuel < 10%)

3 – MEDIUM (commercial flights)

4 – LOW (cargo / private jets)

Operations:

Request landing → insert into heap

Process landing → extractMin()

Declare emergency / low fuel → decreaseKey() to bump priority

Colored landing queue display (priority and fuel level highlighted).

Aircraft Registry (Module C)
Hash Table with chaining (linked lists) for O(1) average lookup by Flight ID.

Stores full aircraft state:

Flight ID, model, fuel, origin, destination

Priority, current graph node, radar coordinates, status (in air / landed)

Operations:

Insert new flight

Search flight by ID

Optional removal

Get all in-air flights (for radar and collision checks)

File I/O: Save and load all flights to/from flights.txt.

Flight Log History (Module D)
AVL Tree (self-balancing BST) keyed by landing timestamp.

Each node stores:

Snapshot of the Aircraft

Flight status string (e.g., LANDED)

Guarantees O(log n) insert/search via LL, RR, LR, RL rotations.

In-order traversal prints a chronological report of the day’s landed flights.

File I/O: Save and load logs to/from flightlog.txt.

Collision Avoidance (Module E)
Before spawning or moving a plane:

Collect all in-air flights from the hash table

Check if target (x, y) grid coordinate is already occupied

If occupied, system raises a collision alert and rejects the move/spawn.

Menu-Driven System
Main menu options:

Airspace Visualizer

Shows radar grid with all planes and nodes

Shows head of landing queue (“Next to Land”)

Flight Control

Add Flight (spawn new plane at entry node)

Move Flight (change node; updates grid & fuel)

Declare Emergency (update to Priority 1 or 2)

Land Flight (process top of Min-Heap, update log)

Search & Reporting

Search Flight (Hash Table lookup)

Print Flight Log (AVL Tree in-order traversal)

Find Safe Route (Dijkstra to nearest airport)

System

Save Data (Hash Table + AVL Tree → text files)

Load Data (restore previous state)

Exit (optionally autosave)

🧱 Project Structure
A typical layout (you can adjust folder names to your repo style):

text
SkyNet_ATC/
├─ include/
│  ├─ Aircraft.h
│  ├─ ATCSystem.h
│  ├─ AVLTree.h
│  ├─ Colors.h
│  ├─ Constants.h
│  ├─ Exceptions.h
│  ├─ Graph.h
│  ├─ HashTable.h
│  ├─ MinHeap.h
│  ├─ Radar.h
│  └─ Utilities.h
│
├─ src/
│  ├─ Aircraft.cpp
│  ├─ ATCSystem.cpp
│  ├─ AVLTree.cpp
│  ├─ Graph.cpp
│  ├─ HashTable.cpp
│  ├─ MinHeap.cpp
│  ├─ Radar.cpp
│  ├─ Utilities.cpp
│  └─ main.cpp
│
├─ data/          # created at runtime
│  ├─ flights.txt
│  └─ flightlog.txt
│
└─ README.md      # this file
🧮 Data Structures Overview
Aircraft (struct)
Core record type storing each plane’s state.

Graph (Adjacency List)

Nodes: Airports & waypoints

Edges: Flight corridors with weights (distance / fuel cost)

Used by Dijkstra to compute shortest path to nearest airport.

Radar (2D char array)

Visual grid mapping nodes & planes to coordinates.

Min-Heap (array-based)

Landing queue sorted by priority (1–4).

Used for landing order and emergency re-prioritization.

Hash Table (with chaining)

Flight registry keyed by Flight ID.

Provides O(1) average search, insert, and update.

AVL Tree

Balanced flight log keyed by landing timestamp.

In-order traversal yields chronological landing report.

🖥️ Build & Run Instructions (Windows / Visual Studio)
Prerequisites
Windows

Visual Studio (any recent version)

C++ toolchain with support for C++11 or higher

Steps (Visual Studio IDE)
Create Project

Open Visual Studio → Create a new project

Select Empty Project (C++ Console)

Name it e.g. SkyNet_ATC

Add Source and Header Files

Right-click Header Files → Add → Existing Item… → add all .h files from include/

Right-click Source Files → Add → Existing Item… → add all .cpp files from src/

Set Language Standard

Project → Properties → C/C++ → Language → C++ Language Standard

Set to ISO C++11 or higher.

Preprocessor Definitions

Project → Properties → C/C++ → Preprocessor:

Add _CRT_SECURE_NO_WARNINGS to disable “unsafe function” warnings for C APIs.

If using windows.h and std::numeric_limits::max, also add NOMINMAX or define it in Colors.h before including <windows.h>.

Build

Build → Build Solution (Ctrl+Shift+B)

Run

Debug → Start Without Debugging (Ctrl+F5)

🎮 How to Use (Quick Demo Flow)
Start the program → main menu appears.

Choose “Add New Flight” → enter flight ID, model, fuel, origin, destination, priority, node ID.

Choose “View Radar” → see airports, waypoints, and planes on 20×20 grid; see “Next to Land.”

Choose “View Landing Queue” → see Min-Heap-based priority queue.

Choose “Declare Emergency” for a flight → priority changes to CRITICAL/HIGH.

Choose “Move Flight” → move a plane to another node; collision check and fuel update happen.

Choose “Land Flight” → top priority plane lands, removed from heap and logged in AVL.

Choose “Search Flight” → lookup by Flight ID from Hash Table.

Choose “Print Flight Log” → historical landings from AVL Tree in chronological order.

Choose “Find Safe Route” → Dijkstra to nearest airport from current node.

Choose “Save Data” → saves to flights.txt and flightlog.txt.

Choose “Load Data” later → restores state from those files.

🧪 Design Constraints (from assignment)
Language: C++11+

No STL containers for core data structures:

Graph, Heap, Hash Table, AVL implemented manually with arrays and pointers.

Console-based visual output using:

system("cls") / system("clear") for refreshing

Windows console color API for colored UI

📌 Possible Future Improvements
Support for multiple runways and separate heaps per runway.

Better path visualization (e.g., highlighting route on radar).

Logging additional events (crashes, diversions) in AVL tree.

Configurable graph from an input file instead of hard-coded nodes/edges.

Portability layer for non-Windows terminals (replace windows.h color handling).

🧑‍💻 Author / Credits
Developed as a Data Structures semester project.

Core concepts implemented manually:

Graph (Dijkstra), Min-Heap, Hash Table with chaining, AVL Tree, 2D grid radar.
