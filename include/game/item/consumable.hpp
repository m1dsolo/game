#pragma once

#include <game/inventory.hpp>
#include <game/item/item.hpp>

namespace wheel {

class Consumable : public Item {
public:
    enum class Type {
        POTION,
    };

    virtual ~Consumable() = default;

    struct Data : Item::Data {
        Data(const std::string& name, const std::string& description, int max_uses = 1)
            : Item::Data(name, description, Item::Type::CONSUMABLE, true), max_uses(max_uses) {}

        Type type;
        int max_uses;
    };

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

    Slot& slot() { return slot_; }
    int& uses() { return uses_; }

protected:
    Consumable(Data* data, Entity entity, Slot& slot) : Item(data, entity), slot_(slot), uses_(data->max_uses) {}

    Slot& slot_;
    int uses_;
};

}  // namespace wheel
