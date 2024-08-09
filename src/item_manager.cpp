#include <game/item_manager.hpp>

#include <ranges>

#include <wheel/json.hpp>
#include <wheel/ecs.hpp>
#include <wheel/utils.hpp>

#include <game/global.hpp>
#include <game/buff_manager.hpp>

#include <game/item/material.hpp>
#include <game/item/weapon.hpp>
#include <game/item/gun.hpp>
#include <game/item/consumable.hpp>
#include <game/item/potion.hpp>
#include <game/item/tower.hpp>
#include <game/item/structure.hpp>
#include <game/item/floor.hpp>

namespace wheel {

ItemManager::ItemManager() {
    parse_item_json();
}

// TODO: reflection
std::shared_ptr<Item> ItemManager::create_item(const std::string& name, Entity entity) {
    auto data = item_data_map[name].get();
    switch (data->type) {
        case Item::Type::NONE: {
            return nullptr;
        }
        case Item::Type::MATERIAL: {
            auto material_data = static_cast<Material::Data*>(data);
            return std::make_shared<Material>(material_data);
        }
        case Item::Type::WEAPON: {
            auto weapon_data = static_cast<Weapon::Data*>(data);
            switch (weapon_data->type) {
                case Weapon::Type::GUN: {
                    auto gun_data = static_cast<Gun::Data*>(weapon_data);
                    return std::make_shared<Gun>(gun_data, entity);
                }
            }
        }
        case Item::Type::CONSUMABLE: {
            auto consumable_data = static_cast<Consumable::Data*>(data);
            switch (consumable_data->type) {
                case Consumable::Type::POTION: {
                    auto potion_data = static_cast<Potion::Data*>(consumable_data);
                    return std::make_shared<Potion>(potion_data, entity);
                }
                case Consumable::Type::TOWER: {
                    auto tower_data = static_cast<Tower::Data*>(consumable_data);
                    return std::make_shared<Tower>(tower_data, entity);
                }
                case Consumable::Type::STRUCTURE: {
                    auto structure_data = static_cast<Structure::Data*>(consumable_data);
                    return std::make_shared<Structure>(structure_data, entity);
                }
                case Consumable::Type::FLOOR: {
                    auto floor_data = static_cast<Floor::Data*>(consumable_data);
                    return std::make_shared<Floor>(floor_data, entity);
                }
            }
        }
    }

    return nullptr;
}

void ItemManager::parse_item_json() {
    JsonListType obj = Json::parse_file(std::format("{}/item.json", game_resource.path));
    // TODO: id
    for (auto [id, a] : std::views::enumerate(obj)) {
        JsonDictType& item = a;
        std::string name = item["name"];
        std::string full_type = item["type"];
        auto sub_types = Utils::split(full_type, '.');
        if (sub_types[0] == "material") {
            auto data = std::make_shared<Material::Data>(name, "");
            item_data_map[name] = data;
        } else if (sub_types[0] == "weapon") {
            if (sub_types[1] == "gun") {
                auto data = std::make_shared<Gun::Data>(name, "");
                data->atk = item["atk"];
                data->penetration = item.count("penetration") ? item["penetration"] : 1;
                data->count = item.count("count") ? item["count"] : 1;
                data->clip = item["clip"];
                data->shoot_interval = item["shoot_interval"];
                data->reload_interval = item["reload_interval"];
                data->angle = item["angle"];
                item_data_map[name] = data;
            }
        } else if (sub_types[0] == "consumable") {
            int max_uses = item.count("max_uses") ? item["max_uses"] : 1;
            if (sub_types[1] == "potion") {
                std::vector<Buff> buffs;
                for (JsonDictType& buff_obj : static_cast<JsonListType&>(item["buffs"])) {
                    std::string buff_name = buff_obj["name"];
                    int value = buff_obj["value"];
                    auto buff = BuffManager::instance().create(buff_name, value);
                    buffs.emplace_back(buff);
                }
                auto data = std::make_shared<Potion::Data>(name, "", max_uses, buffs);
                item_data_map[name] = data;
            } else if (sub_types[1] == "tower") {
                int power = item["power"];
                auto data = std::make_shared<Tower::Data>(name, "", max_uses, power);
                item_data_map[name] = data;
            } else if (sub_types[1] == "structure") {
                auto data = std::make_shared<Structure::Data>(name, "", max_uses);
                item_data_map[name] = data;
            } else if (sub_types[1] == "floor") {
                auto data = std::make_shared<Floor::Data>(name, "", max_uses);
                item_data_map[name] = data;
            }
        }
    }
}

}  // namespace wheel
