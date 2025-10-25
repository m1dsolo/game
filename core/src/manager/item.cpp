#include <core/manager/item.hpp>

#include <rfl/json.hpp>

namespace core {

ItemManager::ItemManager() {
    set({"", Rarity::common});

    std::ifstream file("assets/config/item.json");
    if (file.is_open()) {
        std::cout << "[begin load item...]" << std::endl;
        auto result = rfl::json::read<std::vector<ItemConfig>>(file);
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

const ItemConfig& ItemManager::get(wheel::ID id) const {
    auto iter = item_infos_.find(id);
    if (iter != item_infos_.end()) {
        return iter->second;
    }
    return item_infos_.at("");
}

const ItemConfig& ItemManager::set(const ItemConfig& item_config) {
    return item_infos_[item_config.name] = item_config;
}

bool ItemManager::has(wheel::ID id) const {
    return item_infos_.find(id) != item_infos_.end();
}

}  // namespace core
