# PACMAN - C++ and SDL

<img src="./demo/main-menu.png" width="350"> <img src="./demo/game.png" width="350">

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

## Pending TODO:
- Clean code in general.
- Add sounds.
- Add maze assets (removing the color rects).
- Add ghosts score animation.
- Create animations class.