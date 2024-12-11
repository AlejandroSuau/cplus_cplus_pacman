# PACMAN - C++ and SDL

![alt text](https://github.com/AlejandroSuau/cplus_cplus_pacman/blob/main/demo/main-menu.png "Main Menu")
![alt text](https://github.com/AlejandroSuau/cplus_cplus_pacman/blob/main/demo/game.png "Game")

## TODO:
- Clean code in general.
- Add sounds.
- Add maze assets (removing the color rects).
- Add ghosts score animation.
- Create animations class.

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
