#pragma once

#include <game/item/consumable.hpp>

namespace wheel {

class Floor : public Consumable {
public:
    struct Data : Consumable::Data {
        Data(const std::string& name, const std::string& description, int max_uses = 1, bool movable = false)
            : Consumable::Data(name, description, Consumable::Type::FLOOR, true, max_uses) {}
    };

    Floor(Data* data, Entity entity) : Consumable(data, entity, "plant_structure") {}

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

    bool use() override;
};

}  // namespace wheel
