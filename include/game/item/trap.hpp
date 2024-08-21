#pragma once

#include <game/item/consumable.hpp>

namespace wheel {

class Trap : public Consumable {
public:
    struct Data : Consumable::Data {
        Data(const std::string& name, const std::string& description, int max_uses = 1, int atk = 1, int durability = 100, int cooldown = 30)
            : Consumable::Data(name, description, Consumable::Type::TOWER, true, max_uses), atk(atk), durability(durability), cooldown(cooldown) {}

        int atk;
        int durability;
        int cooldown;
    };

    Trap(Data* data, Entity entity) : Consumable(data, entity, "plant_trap") {}

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

    bool use() override;
};

}  // namespace wheel

