<div align="center">

<h1>一个简单的Roguelike游戏</h1>

<p>（还没想好游戏名字。）</p>

[![license](https://img.shields.io/badge/License-MIT%202.0-blue.svg)](https://github.com/Lightning-AI/lightning/blob/master/LICENSE)

</div>

## 语言

- [English](README.md)
- [简体中文](README_zh_CN.md)

## 目录

- [介绍](#介绍)
- [特点](#特点)
- [安装](#安装)
- [使用](#使用)
- [许可证](#许可证)

## 介绍

演示：

[演示](https://github.com/user-attachments/assets/bb18c641-a574-485e-b261-53d6e00e250e)

这是基于 [SDL3](https://github.com/libsdl-org/SDL) 和我的 [ECS](https://github.com/m1dsolo/wheel/blob/main/include/wheel/ecs.hpp) 框架开发的一个简单的Roguelike游戏。

这款游戏是一个2D顶视角的Roguelike生存游戏。
目标是在无尽的敌人波次中生存下来。
在每个波次开始之前，你需要升级敌人。
然后，你有半分钟的时间来抵御敌人的猛烈攻击。
在此期间，你可以升级自己，收集资源，并建立强大的防御结构。

由于游戏基于高度可扩展的 `ECS` 框架，
并且主要的游戏信息可以通过 `JSON` 配置，
你可以轻松地添加更多的怪物、物品以及修改AI等内容。

## 特点

项目使用了以下技术：

- `ECS`（实体-组件-系统）架构，提供高度可复用的模块和高效的执行速度
- `QuadTree` 加速碰撞检测
- `SceneGraph` 管理对象之间的逻辑关系和渲染顺序
- `BehaviorTree` 管理敌人和防御塔的AI逻辑
- `Lazy Theta*` 寻路算法，提供比 `A*` 更平滑的移动效果
- 使用 `JSON` 配置游戏设置、怪物信息、物品信息、升级奖励、怪物升级信息、AI等

`ECS`、`QuadTree` 和其他模块主要依赖我的 [wheel 库](https://github.com/m1dsolo/wheel)。
更多细节请参考该库的文档。

## 安装

该项目使用 `C++23` 编写，并使用 `CMake` 构建。
目前仅在我自己的电脑（Arch Linux）上进行了测试。
如果你使用其他操作系统，可能需要修改 `CMakeLists.txt` 以满足你的需求。

1. 克隆项目

```bash
git clone --depth=1 https://github.com/m1dsolo/game.git
cd game
git submodule update --init --depth=1
```

2. 编译

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

3. 运行

```bash
./build/game
```

## 使用

游戏控制如下：
- 移动
    - 使用 `WASD` 键移动角色
- 战斗
    - 选择枪支后使用 `左键` 射击
    - 选择枪支后使用 `R` 键重新装弹
    - 选择消耗品后使用 `空格键` 使用
    - 使用 `F` 键闪现
    - 使用 `G` 键丢弃选中的物品
- 快捷栏
    - 使用 `1`、`2`、`3` 等键选择物品
    - 使用 `QE` 键切换物品
    - 按住 `左键` 交换物品
- 菜单
    - 使用 `ESC` 键显示退出菜单
        - `1` 键退出游戏
        - `2` 键继续游戏
    - 使用 `I` 键切换背包菜单
        - 控制与快捷栏相同

## 许可证

[MIT](LICENSE) © m1dsolo
