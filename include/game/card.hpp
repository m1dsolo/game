#pragma once

#include <memory>
#include <string>
#include <vector>

#include <wheel/singleton.hpp>

#include <game/buff_manager.hpp>

namespace wheel {

class Card {
public:
    Card(const std::string& name, std::vector<Buff> buffs) : name_(name), buffs_(buffs) {}

    void use();

    const std::string& name() const { return name_; }
    const std::vector<Buff>& buffs() const { return buffs_; }

private:
    std::string name_;
    std::vector<Buff> buffs_;
};

class CardManager : public Singleton<CardManager> {
public:
    CardManager();

    std::vector<std::shared_ptr<Card>>& player_cards() { return player_cards_; }
    std::vector<std::shared_ptr<Card>>& enemy_cards() { return enemy_cards_; }

    void parse_player_card(const std::string& path);
    void parse_enemy_card(const std::string& path);

private:
    std::vector<std::shared_ptr<Card>> player_cards_;
    std::vector<std::shared_ptr<Card>> enemy_cards_;
};

}  // namespace wheel
