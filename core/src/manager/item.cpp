#include <core/manager/item.hpp>
#include <core/reflector/string_to_id.hpp>

#include <rfl/json.hpp>

namespace core {

ItemManager::ItemManager() {
    set({""});

    std::ifstream file("assets/config/item.json");
    if (file.is_open()) {
        std::cout << "[begin load item...]" << std::endl;
        auto result = rfl::json::read<std::vector<ItemConfig>, rfl::DefaultIfMissing>(file);
        if (result.has_value()) {
            for (const auto& item_config : result.value()) {
                set(item_config);
            }
        } else {
            std::cerr << "Failed to load item config: " << result.error().what() << std::endl;
        }
        std::cout << "[load item done]" << std::endl;
    }
}

bool ItemManager::has(wheel::ID id) const {
    return item_infos_.find(id) != item_infos_.end();
}

const ItemConfig& ItemManager::get(wheel::ID id) const {
    auto iter = item_infos_.find(id);
    if (iter != item_infos_.end()) {
        return iter->second;
    }
    return item_infos_.at("");
}

const ItemConfig& ItemManager::set(const ItemConfig& item_config) {
    wheel::ID id = wheel::ID(item_config.name) + static_cast<int>(item_config.rarity);
    auto [it, _] = item_infos_.try_emplace(id, item_config);
    if (item_config.sprite.has_value()) {
        it->second.sprite_id = *item_config.sprite;
    } else {
        it->second.sprite_id = item_config.name;
    }
    return it->second;
}

}  // namespace core
