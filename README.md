## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Install](#install)
- [Usage](#usage)
- [License](#license)

## Introduction

Demo:

[demo](https://github.com/user-attachments/assets/c22e5a91-4854-4831-b469-1eb88cf598ba)

This is a simple game developed based on my [ECS](https://github.com/m1dsolo/wheel/blob/main/include/wheel/ecs.hpp) framework.
The game mainly relies on SDL3 and uses on almost no other external libraries.
In the process of making the game, I mainly rely on my wheel library.
Please refer to the [wheel library](https://github.com/m1dsolo/wheel) for more details.

This game is currently a survivor-like game, but I will randomly add elements that I like.
For example:
1. Construction system
2. Farming system
3. Store system
4. Multiplayer system
5. etc...

Update:
1. consumable potion

## Features

- Entity-Component-System (ECS) architecture
- Primarily uses the SDL3 library for graphics and input handling
- No external libraries except SDL3
- json configuration game information

## Install

This project is written in c++23 and built using cmake.
I have only tested it on my own computer (Arch Linux).
If you use another operating systems, you may need to modify `CMakeLists.txt` to meet your needs.

1. clone project

```bash
git clone --depth=1 https://github.com/m1dsolo/game.git
cd game
git submodule update --init --depth=1
```

2. build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

3. run

```bash
./build/game
```

## Usage

1. Use `WASD` to move the character
2. Press `Space` to shoot if you have a gun selected
3. Press `R` to reload the gun if you have one selected
4. Press `ESC` to show the exit menu (then press `1` to quit, `2` to resume)
5. Use the number keys `1`, `2`, `3`, etc. to select items
6. Use the number keys `1`, `2`, `3` to select the levelup reward cards
7. Use `Space` to use consumables if you have one selected

## License

[MIT](LICENSE) © m1dsolo
