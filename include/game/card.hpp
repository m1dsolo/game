#pragma once

#include <string>
#include <vector>

#include <wheel/singleton.hpp>

#include <game/buff_manager.hpp>

namespace wheel {

class Card {
public:
    Card(const std::string& name, int rarity, std::vector<Buff> buffs) : name_(name), rarity_(rarity), buffs_(buffs) {}

    void use();

    const std::string& name() const { return name_; }
    int rarity() const { return rarity_; }
    const std::vector<Buff>& buffs() const { return buffs_; }

private:
    std::string name_;
    int rarity_;
    std::vector<Buff> buffs_;
};

}  // namespace wheel
