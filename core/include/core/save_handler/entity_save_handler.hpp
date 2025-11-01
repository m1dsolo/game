#pragma once

#include <core/save_handler/save_handler.hpp>
#include <core/global.hpp>
#include <core/reflector/vector2d.hpp>
#include <core/reflector/sprite_component.hpp>
#include <core/reflector/animation_component.hpp>
#include <core/reflector/animation_fsm_component.hpp>
#include <core/reflector/rect.hpp>
#include <core/reflector/circle.hpp>
#include <core/reflector/id.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/collider.hpp>
#include <core/manager/render.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/collider.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/sprite.hpp>
#include <core/component/animation.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/component/render.hpp>
#include <core/component/parent.hpp>
#include <core/component/children.hpp>
#include <core/component/master.hpp>
#include <core/component/hp.hpp>
#include <core/component/aura_damage.hpp>
#include <core/component/attack.hpp>
#include <core/component/range_attack.hpp>
#include <core/component/reload.hpp>
#include <core/component/inventory.hpp>
#include <core/component/item.hpp>
#include <core/component/loot.hpp>
#include <core/component/level.hpp>
#include <core/component/projectile.hpp>
#include <core/component/track.hpp>
#include <core/component/text.hpp>
#include <core/tag/root.hpp>
#include <core/tag/camera.hpp>
#include <core/tag/input.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/tag/render.hpp>
#include <core/tag/obstacle.hpp>
#include <core/tag/hp_bar.hpp>
#include <core/tag/absorb_item.hpp>
#include <core/tag/pickup_item.hpp>
#include <core/tag/game_layer.hpp>
#include <core/tag/save.hpp>
#include <core/util/utils.hpp>

#include <wheel/log.hpp>
#include <rfl/json.hpp>

#include <ranges>

namespace core {

template <typename... ExtraComponentTypes>
class EntitySaveHandler : public SaveHandler {
public:
    void save() override;
    void load() override;

private:
    rfl::Generic::Object serialize_entity_(wheel::Entity entity);

    wheel::Entity deserialize_entity_(const rfl::Generic::Object& obj);

    template <typename... AllComponentTypes>
    void serialize_components_(wheel::Entity entity, rfl::Generic::Object& obj) {
        (serialize_component_<AllComponentTypes>(entity, obj), ...);
    }

    template <typename ComponentType>
    void serialize_component_(wheel::Entity entity, rfl::Generic::Object& obj) {
        if (ecs.has_component<ComponentType>(entity)) {
            obj[Utils::get_type_name<ComponentType>()] = rfl::to_generic(ecs.get_component<ComponentType>(entity));
        }
    }

    template <typename... AllComponentTypes>
    void deserialize_components_(wheel::Entity entity, const rfl::Generic::Object& obj) {
        (deserialize_component_<AllComponentTypes>(entity, obj), ...);
    }

    template <typename ComponentType>
    void deserialize_component_(wheel::Entity entity, const rfl::Generic::Object& obj) {
        if (auto res = obj.get(Utils::get_type_name<ComponentType>()); res.has_value()) {
            ecs.add_component(entity, rfl::from_generic<ComponentType>(res.value()).value());
        }
    }
};

template <typename... ExtraComponentTypes>
void EntitySaveHandler<ExtraComponentTypes...>::save() {
    std::ofstream file("entities.json");
    if (file.is_open()) {
        rfl::Generic::Array array;
        for (auto entity : ecs.get_entities<SaveTag>()) {
            array.emplace_back(rfl::to_generic(serialize_entity_(entity)));
        }
        file << rfl::json::write(array, rfl::json::pretty);
        file.close();
    }
}

template <typename... ExtraComponentTypes>
void EntitySaveHandler<ExtraComponentTypes...>::load() {
    EntityManager::instance().remove_entity(ecs.get_entity<CameraTag>());

    std::unordered_map<wheel::Entity, wheel::Entity> entity_map;
    auto json_content = Utils::read_file("entities.json");
    auto array = rfl::json::read<rfl::Generic::Array>(json_content).value();
    for (const auto& generic : array) {
        auto obj = generic.to_object().value();
        auto old_entity = rfl::from_generic<wheel::Entity>(obj.get("entity").value()).value();
        auto new_entity = deserialize_entity_(obj);
        entity_map[old_entity] = new_entity;
    }

    auto& root_children = ecs.get_component<ChildrenComponent>(ecs.get_entity<RootTag>()).entities;
    for (auto [old_entity, new_entity] : entity_map) {
        wheel::Log::debug("[load] Update entity references: {} -> {}", old_entity, new_entity);
        if (ecs.has_component<ParentComponent>(new_entity)) {
            auto& parent = ecs.get_component<ParentComponent>(new_entity);
            if (parent.entity == 0) {
                root_children.emplace_back(new_entity);
            } else if (auto it = entity_map.find(parent.entity); it != entity_map.end()) {
                parent.entity = it->second;
            } else {
                wheel::Log::warning("[load] Parent entity {} not found for entity {}", parent.entity, new_entity);
            }
        }
        if (ecs.has_component<ChildrenComponent>(new_entity)) {
            auto& children = ecs.get_component<ChildrenComponent>(new_entity);
            std::vector<wheel::Entity> entities;
            auto new_entities = children.entities |
                std::views::filter([&entity_map](auto e) { return entity_map.contains(e); }) |
                std::views::transform([&entity_map](auto e) { return entity_map[e]; }) |
                std::ranges::to<std::vector>();
            children.entities = std::move(new_entities);
        }
        if (ecs.has_component<MasterComponent>(new_entity)) {
            auto& master = ecs.get_component<MasterComponent>(new_entity);
            if (auto it = entity_map.find(master.entity); it != entity_map.end()) {
                master.entity = it->second;
            } else {
                wheel::Log::warning("[load] Master entity {} not found for entity {}", master.entity, new_entity);
            }
        }
        if (ecs.has_component<ProjectileComponent>(new_entity)) {
            auto& projectile = ecs.get_component<ProjectileComponent>(new_entity);
            if (auto it = entity_map.find(projectile.last); it != entity_map.end()) {
                projectile.last = it->second;
            } else {
                wheel::Log::warning("[load] Projectile last entity {} not found for entity {}", projectile.last, new_entity);
            }
        }
        if (ecs.has_component<TrackComponent>(new_entity)) {
            auto& track = ecs.get_component<TrackComponent>(new_entity);
            if (auto it = entity_map.find(track.target); it != entity_map.end()) {
                track.target = it->second;
            } else {
                wheel::Log::warning("[load] Track target entity {} not found for entity {}", track.target, new_entity);
            }
        }
        if (ecs.has_component<TextComponent>(new_entity)) {
            auto& text = ecs.get_component<TextComponent>(new_entity);
            EntityManager::instance().update_text(new_entity, "");
        }
    }
}

template <typename... ExtraComponentTypes>
rfl::Generic::Object EntitySaveHandler<ExtraComponentTypes...>::serialize_entity_(wheel::Entity entity) {
    rfl::Generic::Object obj;
    obj["entity"] = entity;
    serialize_components_<
        NameComponent,
        TransformComponent,
        ColliderComponent,
        DirectionComponent,
        SpeedComponent,
        SpriteComponent,
        AnimationComponent,
        AnimationFSMComponent,
        RenderComponent,
        ParentComponent,
        ChildrenComponent,
        MasterComponent,
        HPComponent,
        AuraDamageComponent,
        AttackComponent,
        RangeAttackComponent,
        ReloadComponent,
        InventoryComponent,
        ItemComponent,
        LootComponent,
        LevelComponent,
        ProjectileComponent,
        TrackComponent,
        TextComponent,
        CameraTag,
        InputTag,
        RigidbodyTag,
        RenderTag,
        ObstacleTag,
        HPBarTag,
        AbsorbItemTag,
        PickupItemTag,
        GameLayerTag,
        SaveTag,
        ExtraComponentTypes...
    >(entity, obj);
    return obj;
}

template <typename... ExtraComponentTypes>
wheel::Entity EntitySaveHandler<ExtraComponentTypes...>::deserialize_entity_(const rfl::Generic::Object& obj) {
    auto entity = ecs.add_entity();
    deserialize_components_<
        NameComponent,
        TransformComponent,
        ColliderComponent,
        DirectionComponent,
        SpeedComponent,
        SpriteComponent,
        AnimationComponent,
        AnimationFSMComponent,
        RenderComponent,
        ParentComponent,
        ChildrenComponent,
        MasterComponent,
        HPComponent,
        AuraDamageComponent,
        AttackComponent,
        RangeAttackComponent,
        ReloadComponent,
        InventoryComponent,
        ItemComponent,
        LootComponent,
        LevelComponent,
        ProjectileComponent,
        TrackComponent,
        TextComponent,
        CameraTag,
        InputTag,
        RigidbodyTag,
        RenderTag,
        ObstacleTag,
        HPBarTag,
        AbsorbItemTag,
        PickupItemTag,
        GameLayerTag,
        SaveTag,
        ExtraComponentTypes...
    >(entity, obj);

    if (ecs.has_component<ColliderComponent>(entity)) {
        ColliderManager::instance().add(entity);
    }

    if (ecs.has_component<RenderTag>(entity)) {
        RenderManager::instance().add(entity);
    }

    return entity;
}

}  // namespace core
