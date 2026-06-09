# 🍬 Candy Crush Engine — C++ & SFML

An interactive, graphic-based 2D grid puzzle engine built in native C++ utilizing the **SFML (Simple and Fast Multimedia Library)** framework. Developed as a Programming Fundamentals semester project at FAST-NUCES, this application simulates standard match-3 architectural loops, dynamic board matrix state updates, and continuous player lifecycle serialization.

## 🚀 Key Features

- **Dynamic Matrix Match Detection:** Implements multi-directional grid traversal algorithms to instantly detect and flag horizontal and vertical alignments of 3 or more uniform types.
- **Advanced Special Candy Spawning System:**
  - ⚡ **Striped Candies:** Created dynamically via lines of 4, rendering a linear sweep marker.
  - 📦 **Wrapped Candies:** Generated through complex overlapping T- or L-shaped matrix patterns.
  - 💣 **Color Bombs:** Triggered by matching 5 elements, executing a recursive global color sweep.
- **Automated Board Refills (Cascading Grid Physics):** Deploys recursive fallback functions that collapse remaining active array rows downward and generate randomized items into cleared upper indices without causing infinite loop lockups.
- **State Serialization & File I/O:** Features full runtime state persistence tracking remaining moves budgets, objective targeting, and active multi-dimensional board coordinates to local flat records (`save.txt`) via standard file streams (`std::ofstream`/`std::ifstream`).
- **Interactive UI Engine:** Coordinates accurate cursor bounding-box tracking and mouse click indexing, translating physical coordinate transforms safely within array matrix memory bounds.

---

## 🕹️ System Architecture & Logic Flow

The software is structured as a centralized game loop managing three main functional layers:
1. **The State Layer:** Multi-dimensional arrays tracking item type values, current score weights, and game status indicators (`isPaused`, `gameOver`, `wonGame`).
2. **The Logic Layer (`markMatches` & `crushCandies`):** Evaluates pattern recognition matrices over a 9x9 board canvas, handles cell state switches, and drives sequential logic steps.
3. **The Presentation Layer (SFML Render Pipeline):** Draws distinct visual shapes, outlines, and unique graphic overlays for special candies dynamically at a locked 60Hz frame rate limit.

---

## 🎮 Game Specifications

- **Grid Dimensions:** 9 × 9 Board Space
- **Tile Resolution:** 60 × 60 Pixels per unit cell
- **Target Win Condition:** Achieve a target score of **1,500 points**
- **Move Constraint:** Must complete within **20 moves**

### In-Game Controls
* 🖱️ **Left Mouse Click:** Select and swap adjacent tiles horizontally or vertically.
* ⌨️ `P` : Pause the game loop
* ⌨️ `R` : Resume active game execution
* ⌨️ `S` : Save current board configuration metrics locally to `save.txt`

---

## 🛠️ Tech Stack & Compilation

- **Language Standard:** C++11 / C++14
- **Graphics Pipeline:** SFML Graphics Module (Window, System)
- **Data Persistence:** Standard C++ Input/Output File Streams (`<fstream>`)

### How to Compile and Run Locally

Ensure you have the SFML development libraries configured on your path environment.

## 🛠️ Compilation and Execution Guide

This engine requires a C++ compiler supporting at least the C++11 standard, along with the **SFML (Simple and Fast Multimedia Library)** development packages.

```bash
# ==============================================================================
# 🐧 OPTION 1: LINUX / UBUNTU SETUP
# ==============================================================================

# 1. Install SFML dependencies via package manager
sudo apt-get update
sudo apt-get install libsfml-dev

# 2. Compile the source code into an object file and link binaries
g++ -c main.cpp -I/usr/include
g++ main.o -o candy_crush -lsfml-graphics -lsfml-window -lsfml-system

# 3. Execute the game engine application
./candy_crush


# ==============================================================================
# 🪟 OPTION 2: WINDOWS SETUP (GCC/MinGW CLI)
# ==============================================================================

# 1. Compile and link the source code using your local MinGW paths
# (Replace "C:\path\to\SFML" with your exact extracted SFML folder path)
g++ -c main.cpp -I"C:\path\to\SFML\include"
g++ main.o -o candy_crush.exe -L"C:\path\to\SFML\lib" -lsfml-graphics -lsfml-window -lsfml-system

# 2. Run the game executable
# (Ensure sfml-graphics-2.dll, sfml-window-2.dll, and sfml-system-2.dll 
# from the SFML 'bin' directory are copied into this same folder first)
candy_crush.exe
