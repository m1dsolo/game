#pragma once

#include <core/system/base.hpp>
#include <core/global.hpp>

#include <wheel/singleton.hpp>

#include <functional>
#include <memory>
#include <vector>

namespace core {

class SystemManager : public wheel::Singleton<SystemManager> {
    friend wheel::Singleton<SystemManager>;

public:
    template <std::derived_from<BaseSystem> T>
    BaseSystem* add_system(bool running) {
        systems_.emplace_back(std::make_unique<T>());
        auto* system = systems_.back().get();
        if (!running) system->pause();
        ecs.add_system(std::bind(&BaseSystem::update, system));
        return system;
    }

    template <std::derived_from<BaseSystem>... Ts>
    void add_systems(bool running) {
        (add_system<Ts>(running), ...);
    }

    // BaseSystem* get(const std::string& name) {
    //     for (auto& system : systems_) {
    //         if (system->name() == name) {
    //             return system.get();
    //         }
    //     }
    //     return nullptr;
    // }
    //
    // std::vector<BaseSystem*> systems() const {
    //     std::vector<BaseSystem*> result;
    //     result.reserve(systems_.size());
    //     std::transform(systems_.begin(), systems_.end(), std::back_inserter(result),
    //                    [](const std::unique_ptr<BaseSystem>& system) { return system.get(); });
    //     return result;
    // }

private:
    SystemManager() = default;
    SystemManager(const SystemManager&) = delete;

    std::vector<std::unique_ptr<BaseSystem>> systems_;
};

}  // namespace core
