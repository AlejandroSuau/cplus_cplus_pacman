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
- Center ghosts on stop (as player) -> CenterAxis();
- If player is on ghost's house's door can't take down direct. Same for ghost in frightened mode.
- In the random choice direction from frightened mode, we can add also to "is_unwated_direction", the direction where the player is. (but what if it is the only one?)

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
