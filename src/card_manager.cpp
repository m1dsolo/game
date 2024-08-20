#include <game/card_manager.hpp>

#include <wheel/json.hpp>

#include <game/resource/enemy.hpp>

namespace wheel {

CardManager::CardManager() {
    parse_player_card(game_resource.path / "player_card.json");
    parse_enemy_card(game_resource.path / "enemy_card.json");

    for (int i = 1; i < 6; i++) {
        rarity_distribution_[i] += rarity_distribution_[i - 1];
    }
}

std::vector<Card*> CardManager::get_random_cards(bool player, int num) {
    if (player) {
        return get_random_player_cards(num);
    } else {
        return get_random_enemy_cards(num);
    }
}

std::vector<Card*> CardManager::get_random_player_cards(int num) {
    std::vector<Card*> cards(num);
    for (int i = 0; i < num; i++) {
        while (true) {
            int rand = random.uniform(0, rarity_distribution_[5]);
            for (int rarity = 1; rarity <= 5; rarity++) {
                if (rand >= rarity_distribution_[rarity - 1] && rand < rarity_distribution_[rarity]) {
                    int card_num = player_cards_[rarity].size();
                    int card_idx = random.uniform(0, card_num - 1);
                    cards[i] = player_cards_[rarity][card_idx].get();
                    break;
                }
            }
            if (!i || std::find(cards.begin(), cards.begin() + i, cards[i]) == cards.begin() + i) {
                break;
            }
        }
    }
    return cards;
}

std::vector<Card*> CardManager::get_random_enemy_cards(int num) {
    std::vector<Card*> cards(num);
    for (int i = 0; i < num; i++) {
        while (true) {
            int card_num = enemy_cards_.size();
            int card_idx = random.uniform(0, card_num - 1);
            cards[i] = enemy_cards_[card_idx].get();
            if (!i || std::find(cards.begin(), cards.begin() + i, cards[i]) == cards.begin() + i) {
                break;
            }
        }
    }
    return cards;
}

void CardManager::parse_player_card(const std::string& path) {
    JsonListType obj = Json::parse_file(path);
    for (JsonDictType card_obj : obj) {
        std::string name = card_obj.at("name");
        int rarity = card_obj.at("rarity");
        std::vector<Buff> buffs;
        for (JsonDictType buff_obj : static_cast<JsonListType&>(card_obj.at("buffs"))) {
            std::string buff_name = buff_obj.at("name");
            int value = buff_obj.at("value");
            auto buff = BuffManager::instance().create(buff_name, value);
            buffs.emplace_back(buff);
        }
        player_cards_[rarity].emplace_back(std::make_shared<Card>(name, rarity, buffs));
    }
}

void CardManager::parse_enemy_card(const std::string& path) {
    JsonListType obj = Json::parse_file(path);
    auto& enemy_resource = ecs.get_resource<EnemyResource>();
    for (JsonDictType card_obj : obj) {
        std::string name = card_obj.at("name");
        std::string key = card_obj.at("key");
        int value = card_obj.at("value");
        Buff buff;
        if (key == "max_hp_ratio") {
            buff = [&enemy_resource, value](Entity) { enemy_resource.max_hp_ratio += value; };
        } else if (key == "max_speed_ratio") {
            buff = [&enemy_resource, value](Entity) { enemy_resource.max_speed_ratio += value; };
        } else if (key == "extra_collide_damage") {
            buff = [&enemy_resource, value](Entity) { enemy_resource.extra_collide_damage += value; };
        } else if (key == "enemy_num") {
            buff = [&enemy_resource, value](Entity) { enemy_resource.enemy_num += value; };
        } else if (key == "elite_chance") {
            buff = [&enemy_resource, value](Entity) { enemy_resource.elite_chance += value; };
        } else if (key == "elite_max_hp_ratio") {
            buff = [&enemy_resource, value](Entity) { enemy_resource.elite_max_hp_ratio += value; };
        } else if (key == "elite_extra_collide_damage") {
            buff = [&enemy_resource, value](Entity) { enemy_resource.elite_extra_collide_damage += value; };
        } else {
            throw std::runtime_error("Unknown key: " + key);
        }
        enemy_cards_.emplace_back(std::make_shared<Card>(name, 0, std::vector{buff}));
    }
}

SDL_Color CardManager::rarity2color(int rarity) {
    return rarity_colors_[rarity];
}

}  // namespace wheel
