#pragma once

#include <core/layer/layer.hpp>

#include <ecs/entity.hpp>
#include <sdl/sdl.hpp>

#include <vector>

namespace core {

struct InventoryComponent;

class InventoryLayer : public core::Layer {
public:
    InventoryLayer() : Layer("InventoryLayer") {}

    void on_register() override;
    void on_show() override;
    void on_hide() override;
    void on_update() override;
    bool on_event(const SDL_Event& event) override;

private:
    inline static constexpr int slot_nums_[2] = { 10, 5 };
    inline static constexpr float slot_sizes_[2] = { 64.f, 64.f };
    inline static constexpr float slot_padding_sizes_[2] = { 5.f, 5.f };
    inline static constexpr float inventory_sizes_[2] = {
        slot_nums_[0] * (slot_sizes_[0] + slot_padding_sizes_[0]) + slot_padding_sizes_[0],
        slot_nums_[1] * (slot_sizes_[1] + slot_padding_sizes_[1]) + slot_padding_sizes_[1]
    };
    inline static std::vector<std::vector<SDL_FRect>> slot_rects_;
};

}  // namespace core
