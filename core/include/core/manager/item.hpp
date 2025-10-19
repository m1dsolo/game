#pragma once

#include <core/config/item.hpp>
#include <core/component/item.hpp>

#include <wheel/singleton.hpp>

#include <unordered_map>
#include <ranges>

namespace core {

class ItemManager : public wheel::Singleton<ItemManager> {
    friend wheel::Singleton<ItemManager>;

public:
    const ItemConfig& get(const std::string& name) const;
    const ItemConfig& set(const ItemConfig& item_config);
    bool has(const std::string& name) const;

    auto item_configs() const {
        return std::views::values(item_infos_);
    }

private:
    ItemManager();
    ItemManager(const ItemManager&) = delete;

    std::unordered_map<std::string, ItemConfig> item_infos_;
};

}  // namespace core
