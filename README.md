Graphical representation of A* search algorithm in 2D grid.
List of developed heuristics: 
- Manhattan distance;
- Diagonal distance;
- Euclidean distance;

The project uses:

- Linux OS;
- Compiler that supports c++17 standard
- SDL2 external library for graphics & keyboard/mouse inputs;
- cmake for build tool;

In order to run the project you need to have the listed libraries installed:
libsdl2-dev;
libsdl2-ttf-dev;
libsdl2-image-dev;
libsdl2-image-mixer;
cmake;

1) To auto-generate na GNU makefile step inside the build directory
and run "cmake .." in the terminal; The cmake should generate a GNU makefile.
2) After generation is complete run "make";
3) When compilation has completed run the binary with "./batman_search";

TODOs:
 - Improve the AStar algorithm with the use of std::priority_queue<>
