#pragma once

#include <core/global.hpp>
#include <core/tag/render.hpp>

#include <ecs/entity.hpp>
#include <ecs/sparse_set.hpp>
#include <wheel/singleton.hpp>

#include <vector>
#include <ranges>

namespace core {

class RenderManager : public wheel::Singleton<RenderManager> {
    friend wheel::Singleton<RenderManager>;

public:
    template <typename... ComponentTypes>
    auto get_entities_ascending() const {
        return layer_entities_ |
            std::views::transform([](const auto& sparse_set) {
                return sparse_set.entities();
            }) |
            std::views::join |
            std::views::filter([&](wheel::Entity entity) {
                return ecs.has_components<RenderTag, ComponentTypes...>(entity);
            });
    }

    template <typename... ComponentTypes>
    auto get_entity_and_components_ascending() const {
        auto entities = get_entities_ascending<ComponentTypes...>();
        return std::views::zip(
            entities,
            entities |
            std::views::transform([&](auto entity) -> ComponentTypes& {
                return ecs.get_component<ComponentTypes>(entity);
            })...
        );
    }

    void add(wheel::Entity entity);

    void remove(wheel::Entity entity);

private:
    RenderManager() = default;
    RenderManager(const RenderManager&) = delete;

    std::vector<wheel::SparseSet<wheel::Entity>> layer_entities_;
};

}  // namespace core
