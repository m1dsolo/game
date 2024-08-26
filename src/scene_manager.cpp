#include <game/scene_manager.hpp>

#include <game/global.hpp>
#include <game/camera.hpp>

#include <game/component/position.hpp>
#include <game/component/size.hpp>
#include <game/component/health_bar.hpp>

namespace wheel {

SceneManager::SceneManager() {
    auto root = std::make_unique<Node>(EntityNone, nullptr, 0);
    root_ = root.get();
    node_map_.insert_or_assign(EntityNone, std::move(root));
}

void SceneManager::add(Entity entity, int priority, Entity parent) {
    // Log::debug("SceneManager::add: entity: {}, parent: {}", entity, parent);
    if (!node_map_.count(entity)) {
        Node* parent_node = node_map_.at(parent).get();
        auto node = std::make_unique<Node>(entity, parent_node, priority);
        Node* node_ptr = node.get();
        parent_node->children.emplace(node_ptr);

        update_node_position(node_ptr);

        nodes_.insert(node_ptr);
        node_map_.insert_or_assign(entity, std::move(node));
    }
}

void SceneManager::remove(Entity entity) {
    // Log::debug("SceneManager::remove: entity: {}", entity);
    if (node_map_.count(entity)) {
        Node* node = node_map_.at(entity).get();
        node->parent->children.erase(node);

        remove(node);
    }
}

std::vector<Entity> SceneManager::query() const {
    return nodes_ | std::views::transform([](const Node* node) { return node->entity; }) | std::ranges::to<std::vector>();
}

void SceneManager::update(Entity entity) {
    if (node_map_.count(entity)) {
        Node* node = node_map_.at(entity).get();

        update_node_and_children_position(node);
    }
}

void SceneManager::remove(Node* node) {
    for (Node* child : node->children) {
        remove(child);
    }
    nodes_.erase(node);
    node_map_.erase(node->entity);
}

// node moving will cause all child node move which has RelativePositionComponent
void SceneManager::update_node_and_children_position(Node* node, const Vector2D<float>& parent_pos) {
    auto pos = update_node_position(node, parent_pos);
    for (Node* child : node->children) {
        update_node_and_children_position(child, pos);
    }
}

Vector2D<float> SceneManager::update_node_position(Node* node, const Vector2D<float>& parent_pos) {
    Entity entity = node->entity;
    auto& position = ecs.get_component<PositionComponent>(entity).vec;
    if (ecs.has_components<RelativePositionComponent>(entity)) {
        const auto& relative_position = ecs.get_component<RelativePositionComponent>(entity).vec;
        if (!parent_pos.is_zero()) {
            position = parent_pos + relative_position;
        } else {
            position = ecs.get_component<PositionComponent>(node->parent->entity).vec + relative_position;
        }
    }
    return position;
}

}  // namespace wheel
