#pragma once

#include <game/item/consumable.hpp>

namespace wheel {

class Structure : public Consumable {
public:
    struct Data : Consumable::Data {
        Data(const std::string& name, const std::string& description, int max_uses = 1, bool movable = false)
            : Consumable::Data(name, description, Consumable::Type::STRUCTURE, true, max_uses) {}
    };

    Structure(Data* data, Entity entity, Slot& slot) : Consumable(data, entity, slot, "plant_structure") {}

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

    bool use() override;
};

}  // namespace wheel
