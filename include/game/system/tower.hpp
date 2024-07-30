#pragma once

#include <game/system/base.hpp>

namespace wheel {

class TowerSystem : public BaseSystem<TowerSystem> {
    friend class BaseSystem<TowerSystem>;

public:
    void execute_impl() override;

private:
    TowerSystem() : BaseSystem(false) {}
    ~TowerSystem() = default;
    TowerSystem(const TowerSystem&) = delete;

    void attack();
};

}  // namespace wheel
