#pragma once

namespace core {

class Game {
public:
    Game();
    virtual ~Game() = default;

    void run();
};

}  // namespace core
