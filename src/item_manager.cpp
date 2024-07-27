#include <game/item_manager.hpp>

#include <ranges>

#include <wheel/json.hpp>
#include <wheel/ecs.hpp>
#include <wheel/utils.hpp>

#include <game/global.hpp>

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
        }
    }
}

}  // namespace wheel
