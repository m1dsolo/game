#pragma once

#include <any>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

#include <wheel/ecs.hpp>
#include <wheel/json.hpp>

namespace wheel {

class BehaviorTree {
public:
    using Context = std::unordered_map<std::string, std::any>;
    struct Node {
        enum class State {
            SUCCESS,
            FAILURE,
            RUNNING,
        };

        virtual State update(Entity entity, Context& context) = 0;
    };
    struct CompositeNode;
    struct SelectorNode;
    struct SequenceNode;
    struct BehaviorNode;

    using Behavior = std::function<Node::State(Entity entity, Context& context)>;

    explicit BehaviorTree(const JsonDictType& json_obj) { load(json_obj); }
    ~BehaviorTree() = default;

    void load(const JsonDictType& json_obj);
    void update(Entity entity);

private:
    std::shared_ptr<Node> root_;
    Entity entity_;

    std::shared_ptr<Node> load_node(const JsonDictType& json_obj);
};

struct BehaviorTree::CompositeNode : BehaviorTree::Node {
    std::vector<std::shared_ptr<Node>> children;
};

struct BehaviorTree::SelectorNode : BehaviorTree::CompositeNode {
    State update(Entity entity, Context& context) override;
};

struct BehaviorTree::SequenceNode : BehaviorTree::CompositeNode {
    State update(Entity entity, Context& context) override;
};

struct BehaviorTree::BehaviorNode : BehaviorTree::Node {
    BehaviorNode(const Behavior& behavior) : behavior(behavior) {}

    State update(Entity entity, Context& context) override;

    BehaviorTree::Behavior behavior;
};

}  // namespace wheel
