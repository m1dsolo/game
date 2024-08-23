#pragma once

#include <memory>
#include <set>
#include <vector>

#include <wheel/singleton.hpp>
#include <wheel/ecs.hpp>
#include <wheel/geometry.hpp>

namespace wheel {

class SceneManager : public Singleton<SceneManager> {
    friend class Singleton<SceneManager>;

public:
    void add(Entity entity, int priority = 0, Entity parent = EntityNone);
    void remove(Entity entity);
    std::vector<Entity> query() const;
    void update(Entity entity);

private:
    SceneManager();
    ~SceneManager() = default;
    SceneManager(const SceneManager&) = delete;

    struct Node {
        Node() {}
        Node(Entity entity, Node* parent, const Rect<float>& rect, int priority)
            : entity(entity), parent(parent), rect(rect), priority(priority) {}

        Entity entity;
        Node* parent;
        Rect<float> rect;
        int priority;

        struct NodePtrCompare {
            bool operator()(const Node* lhs, const Node* rhs) const {
                if (lhs->priority != rhs->priority) {
                    return lhs->priority < rhs->priority;
                }
                return lhs->entity < rhs->entity;
            }
        };
        std::set<Node*, NodePtrCompare> children;
    };

    Node* root_;
    std::unordered_map<Entity, std::unique_ptr<Node>> node_map_;

    void remove(Node* node);
    void query(Node* node, const Rect<float>& rect, std::vector<Entity>& res) const;
    void update_node_rect(Node* node);
    void update_node_and_parents_rect(Node* node);
    Vector2D<float> update_node_position(Node* node, const Vector2D<float>& parent_pos = {});
    void update_node_and_children_position(Node* node, const Vector2D<float>& parent_pos = {});
    void update_node_and_children_rect_and_position(Node* node, const Vector2D<float>& parent_pos = {});
    Rect<float> get_rect(Entity entity) const;
};

}  // namespace wheel 
