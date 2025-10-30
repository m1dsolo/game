#pragma once

#include <core/other/sprite.hpp>

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>
#include <wheel/id.hpp>

#include <unordered_map>

namespace core {

class SpriteManager : public wheel::Singleton<SpriteManager> {
    friend wheel::Singleton<SpriteManager>;

public:
    const Sprite& set(wheel::ID id, const Sprite& sprite);

    const Sprite& get(wheel::ID id) const;

    bool has(wheel::ID id) const;

    void del(wheel::ID id);

private:
    SpriteManager();
    SpriteManager(const SpriteManager&) = delete;

    void init_map_sprites_();
    void init_hp_bar_sprites_();
    void init_slot_item_num_sprites_();
    void init_aura_sprites_();
    void init_auto_shoot_sprites_();
    void init_inventory_sprites_();

    std::unordered_map<wheel::ID, Sprite> id2sprites_;
};

}  // namespace core
