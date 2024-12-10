# PACMAN - C++ and SDL

## TODO:
- Config levels from JSON file.
- Add sounds.
- Add maze assets (removing the color rects).
- Add ghosts score animation.
- Add buttons (sound off / sound on)
- Create animations class.
- On win the game. Stop all ghosts / player. Reproduce sounds. Remove ghosts. Blinky background blues. Start again (without resetting score).

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
