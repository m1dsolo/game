<div align="center">

<h1>A Simple Rougelike Game</h1>

<p>(I haven't thought of a name for this game yet.)</p>

[![license](https://img.shields.io/badge/License-MIT%202.0-blue.svg)](https://github.com/Lightning-AI/lightning/blob/master/LICENSE)

</div>

## Language

- [English](README.md)
- [简体中文](README_zh_CN.md)

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Install](#install)
- [Usage](#usage)
- [License](#license)

## Introduction

Demo:

[demo](https://github.com/user-attachments/assets/bb18c641-a574-485e-b261-53d6e00e250e)

This is a simple rougelike game developed based on [SDL3](https://github.com/libsdl-org/SDL) and my [ECS](https://github.com/m1dsolo/wheel/blob/main/include/wheel/ecs.hpp) framework.

This game is a 2D top-down rougelike survivor roguelike.
The objective is to survive through endless waves of enemies.
Before each wave starts, you need to upgrade the enemies.
Then you have half a minute to survive the relentless attacks of the enemies.
During this time, you can upgrade yourself, collect resources, and establish powerful defensive structures.

Since the game is based on highly scalable `ECS` framework,
and the main game information can be configured through `JSON`,
you can easily add more monsters, items, and modify the AI, etc.

## Features

The project used the following technologies:

- `ECS`(Entity-Component-System) architecture, providing highly reusable modules and efficient execution speed
- `QuadTree` to accelerate collision detection
- `SceneGraph` to manage the logical relationship between objects and the rendering order
- `BehaviorTree` to manage the AI Logic of enemies and defense towers
- `Lazy Theta*` pathfinding algorithm, providing smoother movement compared to `A*`
- `JSON` is used to configure game settings, monster information, item information, upgrade rewards, monster upgrade information, behavior tree, etc.

`ECS`, `QuadTree` and other modules primarily rely on my [wheel library](https://github.com/m1dsolo/wheel).
Refer to the library's documentation for more details.

## Install

This project is written in `C++23` and built using `CMake`.
It has only been tested on my own computer (Arch Linux).
If you use another operating systems, you may need to modify `CMakeLists.txt` to meet your requirements.

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

The game controls are as follows:
- Movement
    - `WASD` to move the character
- Combat
    - `LeftMouse` to shoot if you have a gun selected
    - `R` to reload the gun if you have one selected
    - `Space` to use consumables if you have one selected
    - `F` to flash
    - `G` to drop selected item
- Hotbar
    - `1`, `2`, `3`, etc. to select items
    - `QE` to cycle through items
    - hold `LeftMouse` to swap items
- Menu
    - `ESC` to show the exit menu
        - `1` to quit
        - `2` to resume
    - `I` to switch inventory menu
        - same as hotbar controls

## License

[MIT](LICENSE) © m1dsolo
