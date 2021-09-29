Graphical representation of A* search algorithm on a 2D grid.
List of developed heuristics: 
- Manhattan distance;
- Diagonal distance;
- Euclidean distance;

The project is cross-platform:
It was tested on Linux OS and Windows
It was tested on MacOS 10000 years ago. Few minor modification may be needed

Dependencies:
- SDL2 external library for graphics and keyboard/mouse inputs;
- cmake for build tool;

In order to run the project you need to have the listed libraries installed:
libsdl2-dev;
libsdl2-ttf-dev;
libsdl2-image-dev;
cmake;

1) To auto-generate na makefile step inside the build directory and run 'cmake ..' in the terminal;
1.1) You may need to specify a generator for your toolchain.
     For example to use MinGW on Windows you need to run 'cmake .. -G "MinGW Makefiles"' 
The cmake should generate a makefile.
2) After generation is complete run 'cmake --build .';
3) When compilation has completed run the binary with "./batman_search";

TODOs:
 - Improve the AStar algorithm with the use of std::priority_queue<>
