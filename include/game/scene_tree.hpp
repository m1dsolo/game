#pragma once

#include <memory>
#include <set>
#include <vector>
#include <unordered_map>

#include <wheel/geometry.hpp>

namespace wheel {

class SceneTree {
public:
    SceneTree(const GetRect& get_rect) : get_rect_(get_rect), root_(std::make_unique<Node>()) {
        node_map_.insert({0, root_.get()});
    }

    void add(Entity entity, int priority = 0, const T& parent = -1) {
        if (!node_map_.count(entity)) {
            auto parent_node = node_map_.at(parent);
            auto node = std::make_unique<Node>(entity, parent_node.get(), get_rect_(entity), priority);
            node_map_.insert({entity, node.get()});
            parent_node->children.insert({entity, std::move(node)});

            update_rect(node.get());
        }
    }

    void remove(const T& entity) {
        if (node_map_.count(entity)) {
            auto node = node_map_.at(entity);
            auto parent_node = node->parent;
            parent_node->children.erase(entity);
            node_map_.erase(entity);

            update_rect(node.get());
        }
    }

    // order by priority and entity
    std::vector<T> query(const Rect<float>& rect) const {
        std::vector<T> res;
        for (const auto& [_, child] : root_->children) {
            query(child.get(), rect, res);
        }
        return res;
    }

    void update(const T& entity) {
        if (node_map_.count(entity)) {
            auto node = node_map_.at(entity);
            update_rect(node.get());
        }
    }

private:
    struct Node {
        Node() {}
        Node(const T& entity, Node* parent, const Rect<float>& rect, int priority)
            : entity(entity), parent(parent), rect(rect), priority(priority) {}

        T entity;
        Rect<float> rect;
        Node* parent;
        int priority;

        struct NodePtrCompare {
            bool operator()(const std::unique_ptr<Node>& lhs, const std::unique_ptr<Node>& rhs) const {
                if (lhs->priority != rhs->priority) {
                    return lhs->priority < rhs->priority;
                }
                return lhs->entity < rhs->entity;
            }
        };
        std::set<std::unique_ptr<Node>, NodePtrCompare> children;
    };

    GetRect get_rect_;
    std::unique_ptr<Node> root_;
    std::unordered_map<T, Node*> node_map_;

    void query(Node* node, const Rect<float>& rect, std::vector<T>& res) const {
        if (node->rect.is_overlapping(rect)) {
            res.push_back(node->entity);
            for (const auto& [_, child] : node->children) {
                query(child.get(), rect, res);
            }
        }
    }

    void update_rect(Node* node) {
        auto rect = node->rect;
        while (node->parent != root_) {
            node = node->parent;
            rect = node->rect.merge(rect);
            node->rect = rect;
        }
    }
};

}  // namespace wheel
