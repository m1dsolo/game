#include <game/behavior_tree.hpp>

#include <game/behavior_manager.hpp>

namespace wheel {

void BehaviorTree::load(const JsonDictType& json_obj) {
    root_ = load_node(json_obj);
}

void BehaviorTree::update(Entity entity) {
    Context context;
    root_->update(entity, context);
}

BehaviorTree::Node::State BehaviorTree::SelectorNode::update(Entity entity, Context& context) {
    for (auto& child : children) {
        if (auto state = child->update(entity, context); state != Node::State::FAILURE) {
            return state;
        }
    }
    return Node::State::FAILURE;
}

BehaviorTree::Node::State BehaviorTree::SequenceNode::update(Entity entity, Context& context) {
    for (auto& child : children) {
        if (auto state = child->update(entity, context); state != Node::State::SUCCESS) {
            return state;
        }
    }
    return Node::State::SUCCESS;
}

BehaviorTree::Node::State BehaviorTree::BehaviorNode::update(Entity entity, Context& context) {
    return behavior(entity, context);
}

std::shared_ptr<BehaviorTree::Node> BehaviorTree::load_node(const JsonDictType& json_obj) {
    std::shared_ptr<BehaviorTree::Node> node;
    if (json_obj.at("type") == "selector") {
        auto selector_node = std::make_shared<SelectorNode>();
        for (auto& child : json_obj.at("nodes").get<JsonListType>()) {
            selector_node->children.push_back(load_node(child.get<JsonDictType>()));
        }
        node = selector_node;
    } else if (json_obj.at("type") == "sequence") {
        auto sequence_node = std::make_shared<SequenceNode>();
        for (auto& child : json_obj.at("nodes").get<JsonListType>()) {
            sequence_node->children.push_back(load_node(child.get<JsonDictType>()));
        }
        node = sequence_node;
    } else if (json_obj.at("type") == "behavior") {
        node = std::make_shared<BehaviorNode>(BehaviorManager::instance().get(json_obj.at("name")));
    }

    return node;
}

}  // namespace wheel
