#include <game/scene_manager.hpp>

#include <game/global.hpp>
#include <game/camera.hpp>

#include <game/component/position.hpp>
#include <game/component/size.hpp>
#include <game/component/health_bar.hpp>

namespace wheel {

SceneManager::SceneManager() {
    auto root = std::make_unique<Node>(EntityNone, nullptr, Rect<float>{}, 0);
    root_ = root.get();
    node_map_.insert_or_assign(EntityNone, std::move(root));
}

void SceneManager::add(Entity entity, int priority, Entity parent) {
    Log::debug("SceneManager::add: entity: {}, parent: {}", entity, parent);
    if (!node_map_.count(entity)) {
        Node* parent_node = node_map_.at(parent).get();
        auto node = std::make_unique<Node>(entity, parent_node, get_rect(entity), priority);
        Node* node_ptr = node.get();
        parent_node->children.emplace(node_ptr);

        update_node_position(node_ptr);
        update_node_and_parents_rect(node_ptr);

        node_map_.insert_or_assign(entity, std::move(node));
    }
}

void SceneManager::remove(Entity entity) {
    Log::debug("SceneManager::remove: entity: {}", entity);
    if (node_map_.count(entity)) {
        Node* node = node_map_.at(entity).get();
        node->parent->children.erase(node);
        update_node_and_parents_rect(node->parent);

        remove(node);
    }
}

std::vector<Entity> SceneManager::query() const {
    std::vector<Entity> res;
    for (Node* child : root_->children) {
        query(child, Camera::instance().rect(), res);
    }
    return res;
}

void SceneManager::update(Entity entity) {
    if (node_map_.count(entity)) {
        Node* node = node_map_.at(entity).get();

        update_node_and_children_rect_and_position(node);
        update_node_and_parents_rect(node);
    }
}

void SceneManager::remove(Node* node) {
    for (Node* child : node->children) {
        remove(child);
    }
    node_map_.erase(node->entity);
}

void SceneManager::query(Node* node, const Rect<float>& rect, std::vector<Entity>& res) const {
    if (rect.is_overlapping(node->rect)) {
        if (!node->children.size()) {
            res.emplace_back(node->entity);
        } else if (auto node_rect = get_rect(node->entity); !node_rect.is_zero() && rect.is_overlapping(node_rect)) {
            res.emplace_back(node->entity);
        }
        for (Node* child : node->children) {
            query(child, rect, res);
        }
    }
}

void SceneManager::update_node_rect(Node* node) {
    node->rect = get_rect(node->entity);
    for (Node* child : node->children) {
        node->rect = node->rect.merge(child->rect);
    }
}

void SceneManager::update_node_and_parents_rect(Node* node) {
    while (node && node != root_) {
        update_node_rect(node);
        node = node->parent;
    }
}

// node moving will cause all child node move which has RelativePositionComponent
void SceneManager::update_node_and_children_position(Node* node, const Vector2D<float>& parent_pos) {
    auto pos = update_node_position(node, parent_pos);
    for (Node* child : node->children) {
        update_node_and_children_position(child, pos);
    }
}

void SceneManager::update_node_and_children_rect_and_position(Node* node, const Vector2D<float>& parent_pos) {
    auto pos = update_node_position(node, parent_pos);
    node->rect = get_rect(node->entity);
    for (Node* child : node->children) {
        update_node_and_children_rect_and_position(child, pos);
        node->rect = node->rect.merge(child->rect);
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

Rect<float> SceneManager::get_rect(Entity entity) const {
    if (!ecs.has_entity(entity)) {
        Log::debug("SceneManager::get_rect: no entity {}", entity);
        return {};
    }
    const auto& position = ecs.get_component<PositionComponent>(entity).vec;
    const auto& size = ecs.get_component<SizeComponent>(entity).vec;
    return {position, size};
}

}  // namespace wheel
