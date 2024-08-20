#pragma once

#include <string>
#include <unordered_map>

#include <wheel/singleton.hpp>

#include <game/behavior_tree.hpp>

namespace wheel {

class BehaviorManager : public Singleton<BehaviorManager> {
    friend class Singleton<BehaviorManager>;

public:
    BehaviorTree::Behavior get(const std::string& name);

private:
    BehaviorManager();
    ~BehaviorManager() = default;
    BehaviorManager(const BehaviorManager&) = delete;

    std::unordered_map<std::string, BehaviorTree::Behavior> behavior_map_;
};

}  // namespace wheel
