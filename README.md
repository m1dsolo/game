<div align="center">

<h1>C++23 SDL3 ECS game framework</h1>

[![license](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/m1dsolo/game/blob/main/LICENSE)

</div>

## Table of Contents

- [Introduction](#Introduction)
- [Features](#Features)
- [Quick Start](#Quick-Start)
- [Credits](#Credits)
- [License](#License)

## Introduction

A lightweight, modern game development framework built with `C++23` and `SDL3`,
designed around the `ECS`(Entity Component System) architecture. This repository includes two parts:
- **Core Framework** ([core](./core)): Provides `ECS` and `SDL3` utilities for game development.
- **Survivor** ([survivor](./games/survivor)): A top-down survivor-style game built on the `core` framework.

`Survivor` demo preview:

[demo](https://github.com/user-attachments/assets/8ecfe4d5-e736-4a5f-ba4b-61c413fa4a7d)

project structure:

```txt
game/
├── core/                   # Core ECS Framework
│   ├── include/core/       # Public headers (ECS, AssetManager, RenderManager)
│   ├── src/                # Framework implementation
│   ├── assets/             # Framework assets (fonts, animation finite state machine)
│   └── third_party/        # Dependencies
│       ├── ecs/            # My simple ECS framework
│       ├── wheel/          # My utility library (Log, Timer, QuadTree, etc.)
│       ├── sdl/            # My OO wrapper for SDL3, SDL3_image, SDL3_mixer, SDL3_ttf
│       └── reflect-cpp/    # C++20 reflection library
├── games/                  # Game Projects
│   └── survivor/           # Demo: 2D Top-down survivor game
│       ├── src/            # Game logic (player, enemies, level design)
│       └── assets/         # Game assets (configs, sprites, animations, sounds, music)
├── editor/                 # Game Editor (TODO)
└── LICENSE                 # MIT License
```

## Features

- **Modern C++23**: Leverages latest standards for clean, efficient code.
- **SDL3 Integration**: Hardware-accelerated rendering, audio, input handling, and window management.
- **ECS Architecture**: Decoupled entity-component logic for scalable, maintainable game code

## Quick Start

### Prerequisites

- **C++ compiler**: need C++23 support. (e.g., `g++` or `clang++`)
- **cmake**: ninja is recommended for faster builds.
- **git**: for cloning the repository and submodules.

Note: This project has been tested on Arch Linux. For Windows/macOS, minor modifications to `CMakeLists.txt` may be required.

### Survivor Game

1. clone the repository

```bash
git clone --depth=1 https://github.com/m1dsolo/game.git
cd game
git submodule update --init --depth=1
cd core/third_party/sdl/third_party
git submodule update --init --depth=1 
cd -
```

2. build the project

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build -j8
```

3. launch the `Survivor` game

```bash
cd build/games/survivor
./survivor
```

Game Controls:
| Action | Keyboard | Gampad |
| - | - | - |
| Menu Navigation | W/S | D-Pad Up/Down |
| Character Movement | WASD | Left Stick |
| Open Pause Menu | ECS | Start Button |


### Core Framework

To build your own game with the `core` framework:
Create a new game directory under `./games` (e.g., `./games/your_game`).
Add a `CMakeLists.txt` to your game folder, linking against the core framework:

```cmake
add_executable(your_game src/main.cpp src/Player.cpp src/Enemy.cpp)

target_link_libraries(your_game PRIVATE core)
```

Re-run the build commands (Step 2 above) to compile your game.
Refer to `Survivor`'s [CMakeLists.txt](games/survivor/CMakeLists.txt) for a complete example.

## Credits

Sprites:
- [MiniFolks - Villagers](https://lyaseek.itch.io/minifvillagers)
- [MiniFolks - Villagers 2](https://lyaseek.itch.io/minifvillagers2)
- [MiniFolks - Forest animals](https://lyaseek.itch.io/miniffanimals)

Repositories:
- [reflect-cpp](https://github.com/getml/reflect-cpp)
- [SDL](https://github.com/libsdl-org/SDL)
- [SDL_image](https://github.com/libsdl-org/SDL_image)
- [SDL_mixer](https://github.com/libsdl-org/SDL_mixer)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)

## License

[MIT](LICENSE) © m1dsolo
