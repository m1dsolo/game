#include <game/behavior_tree_manager.hpp>

#include <wheel/json.hpp>

namespace wheel {

void BehaviorTreeManager::load(const std::string& json_path) {
    JsonDictType obj = Json::parse_file(json_path);
    for (auto& [key, val] : obj) {
        behavior_tree_map_.insert_or_assign(key, BehaviorTree(static_cast<JsonDictType&>(val)));
    }
}

BehaviorTree* BehaviorTreeManager::get(const std::string& name) {
    return &behavior_tree_map_.at(name);
}

}  // namespace wheel
