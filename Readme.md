# Flappy Bird - C++ and SDL

## TODO:
1. Ghost inheritance.
2. Player and ghost entity inheritance.
3. Eyes and Frightened should check if is between tiles (as chasing do).
4. Main menu.
5. Config levels from JSON file.

## Technology
Compilation:
* C++20, CMake & Make.

Framework and libraries:
* SDL2
* SDL2_image
* SDL2_mixer
* SDL2_ttf

### Windows - Build and execute

```
cmake -B build`
cd build
cmake --build .
.\Debug\Pacman.exe
```

### MacOS - Build and execute

```
cmake -B build
cd build
make
./Pacman
```
