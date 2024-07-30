#pragma once

#include <game/system/base.hpp>

namespace wheel {

class HandleSDLEventSystem : public BaseSystem<HandleSDLEventSystem> {
    friend class BaseSystem<HandleSDLEventSystem>;

public:
    void execute_impl() override;

private:
    HandleSDLEventSystem() : BaseSystem(true) {}
    ~HandleSDLEventSystem() = default;
    HandleSDLEventSystem(const HandleSDLEventSystem&) = delete;
};

}  // namespace wheel

