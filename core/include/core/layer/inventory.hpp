#pragma once

#include <core/layer/layer.hpp>

#include <ecs/entity.hpp>
#include <sdl/sdl.hpp>

#include <vector>

namespace core {

class InventoryLayer : public Layer {
public:
    InventoryLayer() {}

    void on_register() override;
    void on_attach() override;
    void on_detach() override;
    bool on_event(const SDL_Event& event) override;

private:
    wheel::Entity selected_slot_border_;
    int selected_idx_[2] = { 0, 0 };

    void select_slot_(int di, int dj);
    void update_sprite_(int i, int j);

    std::vector<std::vector<wheel::Entity>> slot_item_entities_;
    std::vector<std::vector<wheel::Entity>> slot_item_rarity_entities_;
    std::vector<std::vector<wheel::Entity>> slot_item_num_entities_;

    inline static constexpr int slot_nums_[2] = { 5, 10 };
    inline static constexpr float slot_sizes_[2] = { 64.f, 64.f };
    inline static constexpr float slot_spacing_sizes_[2] = { 5.f, 5.f };
    inline static std::vector<std::vector<SDL_FRect>> slot_rects_;
};

}  // namespace core
