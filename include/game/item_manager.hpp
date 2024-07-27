#pragma once

#include <game/item/item.hpp>
#include <game/item/material.hpp>
#include <game/item/gun.hpp>
#include <game/inventory.hpp>

#include <wheel/singleton.hpp>

namespace wheel {

class ItemManager : public Singleton<ItemManager> {
    friend class Singleton<ItemManager>;

public:
    // Item
    ItemManager();
    ~ItemManager() = default;
    ItemManager(const ItemManager&) = delete;

    const Item::Data* get_data(const std::string& name) const { return item_data_map.at(name).get(); }

    std::shared_ptr<Item> create_item(const std::string& name, Entity entity = EntityNone);

    void parse_item_json();

    std::unordered_map<std::string, std::shared_ptr<Item::Data>> item_data_map;
};

}  // namespace wheel
