#pragma once

#include <game/inventory.hpp>
#include <game/item/item.hpp>

namespace wheel {

class Consumable : public Item {
public:
    enum class Type {
        POTION,
        TOWER,
        STRUCTURE,
        FLOOR,
    };

    virtual ~Consumable() = default;

    struct Data : Item::Data {
        Data(const std::string& name, const std::string& description, Type type, bool show_selected_tile, int max_uses = 1)
            : Item::Data(name, description, Item::Type::CONSUMABLE, true, show_selected_tile), type(type), max_uses(max_uses) {}

        Type type;
        int max_uses;
    };

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

    int& uses() { return uses_; }

    std::string info() const override {
        auto& d = data();
        return std::format("{}: {}/{}", d.name, uses_, data().max_uses);
    }

    int progress() override { return uses_ * 100 / data().max_uses; }

    virtual bool use() = 0;

    SDL_Texture* get_cursor_texture() const override;

protected:
    Consumable(
        Data* data,
        Entity entity,
        const std::string& action_name,
        std::function<int()> get_cooldown = [] { return 0; },
        SDL_Keycode keycode = ' '
    );

    int uses_;
};

}  // namespace wheel
