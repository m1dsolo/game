#pragma once

#include <string>
#include <unordered_map>

#include <wheel/singleton.hpp>

#include <game/behavior_tree.hpp>

namespace wheel {

class BehaviorTreeManager : public Singleton<BehaviorTreeManager> {
    friend class Singleton<BehaviorTreeManager>;

public:
    void load(const std::string& json_path);

    BehaviorTree* get(const std::string& name);

private:
    BehaviorTreeManager() = default;
    ~BehaviorTreeManager() = default;
    BehaviorTreeManager(const BehaviorTreeManager&) = delete;

    std::unordered_map<std::string, BehaviorTree> behavior_tree_map_;
};

}  // namespace wheel
