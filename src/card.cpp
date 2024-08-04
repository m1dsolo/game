#include <game/card.hpp>

#include <wheel/json.hpp>

#include <game/global.hpp>
#include <game/event.hpp>
#include <game/entity_manager.hpp>

#include <game/component/hp.hpp>
#include <game/component/self.hpp>
#include <game/component/perk.hpp>

#include <game/resource/enemy.hpp>

namespace wheel {

void Card::use() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    for (auto buff : buffs_) {
        buff(entity);
    }
}

CardManager::CardManager() {
    parse_player_card(game_resource.path + "/player_card.json");
    parse_enemy_card(game_resource.path + "/enemy_card.json");
}

void CardManager::parse_player_card(const std::string& path) {
    JsonListType obj = Json::parse_file(path);
    for (JsonDictType card_obj : obj) {
        std::string name = card_obj.at("name");
        std::vector<Buff> buffs;
        for (JsonDictType buff_obj : static_cast<JsonListType&>(card_obj.at("buffs"))) {
            std::string buff_name = buff_obj.at("name");
            int value = buff_obj.at("value");
            auto buff = BuffManager::instance().create(buff_name, value);
            buffs.emplace_back(buff);
        }
        player_cards_.emplace_back(std::make_shared<Card>(name, buffs));
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
        }
        enemy_cards_.emplace_back(std::make_shared<Card>(name, std::vector{buff}));
    }
}

}  // namespace wheel
