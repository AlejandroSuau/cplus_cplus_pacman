# PACMAN - C++ and SDL

## TODO:
- Eyes and Frightened should check if is between tiles (as chasing do).
- Main menu.
- Config levels from JSON file.
- Prevent ghosts from entering in the house while in frightened mode.
- Add sounds.
- Add maze assets (removing the color rects).
- Add ghosts score animation.
- Add buttons (sound off / sound on)
- Create animations class.
- On win the game. Stop all ghosts / player. Reproduce sounds. Remove ghosts. Blinky background blues. Start again (without resetting score).
- Add text renderer to the renderer?

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
