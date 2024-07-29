#pragma once

#include <game/item/consumable.hpp>

namespace wheel {

class Tower : public Consumable {
public:
    struct Data : Consumable::Data {
        Data(const std::string& name, const std::string& description, int max_uses = 1, int power = 50)
            : Consumable::Data(name, description, Consumable::Type::TOWER, max_uses), power(power) {}

        int power;
    };

    Tower(Data* data, Entity entity, Slot& slot) : Consumable(data, entity, slot, "plant_archer_tower") {}

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

    void use() override;
};

}  // namespace wheel
