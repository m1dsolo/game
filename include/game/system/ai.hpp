#pragma once

#include <game/system/base.hpp>

namespace wheel {

class AISystem : public BaseSystem<AISystem> {
    friend class BaseSystem<AISystem>;

public:
    void execute_impl() override;

private:
    AISystem() : BaseSystem(false) {}
    ~AISystem() = default;
    AISystem(const AISystem&) = delete;
};

}  // namespace wheel

