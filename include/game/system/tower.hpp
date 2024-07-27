#pragma once

#include <wheel/singleton.hpp>

namespace wheel {

class TowerSystem : public Singleton<TowerSystem> {
    friend class Singleton<TowerSystem>;

public:
    void execute();

private:
    TowerSystem() = default;
    ~TowerSystem() = default;
    TowerSystem(const TowerSystem&) = delete;

    void attack();
};

}  // namespace wheel
