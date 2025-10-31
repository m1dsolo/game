#pragma once

#include <core/layer/layer.hpp>
#include <core/config/item.hpp>

#include <ecs/entity.hpp>
#include <sdl/sdl.hpp>

namespace core {

class ItemInfoLayer : public Layer {
public:
    ItemInfoLayer() {}

    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    bool on_event(const SDL_Event& event) override;

    inline static wheel::Entity item_entity;
    inline static wheel::Entity selected_slot_border;

private:
    wheel::Entity item_info_entity_ = wheel::NullEntity;
};

}  // namespace core
