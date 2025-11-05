#pragma once

#include <core/config/item.hpp>
#include <core/component/item.hpp>

#include <wheel/singleton.hpp>
#include <wheel/id.hpp>

#include <unordered_map>
#include <ranges>

namespace core {

class ItemManager : public wheel::Singleton<ItemManager> {
    friend wheel::Singleton<ItemManager>;

public:
    bool has(wheel::ID id) const;
    const ItemConfig& get(wheel::ID id) const;
    const ItemConfig& set(const ItemConfig& item_config);

    auto item_configs() const {
        return std::views::values(item_infos_);
    }

private:
    ItemManager();
    ItemManager(const ItemManager&) = delete;

    std::unordered_map<wheel::ID, ItemConfig> item_infos_;
};

}  // namespace core
