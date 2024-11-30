# PACMAN - C++ and SDL

## TODO:
1. Ghost inheritance.
2. Player and ghost entity inheritance.
3. Eyes and Frightened should check if is between tiles (as chasing do).
4. Main menu.
5. Config levels from JSON file.
6. Prevent ghosts from entering in the house while in frightened mode.
7. Add sounds.
8. Add maze assets (removing the color rects).
9. Add ghosts score animation.
10. Add ready label.
11. Pause the game a few seconds after player dead.
12. Add buttons (sound off / sound on)
13. Create animations class.

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
