# PACMAN - C++ and SDL

## TODO:
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
- In the random choice direction from frightened mode, we can add also to "is_unwated_direction", the direction where the player is. (but what if it is the only one?)
- Implement win game.

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
