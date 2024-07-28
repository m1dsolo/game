#pragma once

#include <game/item/item.hpp>

namespace wheel {

class Weapon : public Item {
public:
    virtual ~Weapon() = default;

    enum class Type {
        GUN,
    };

    struct Data : Item::Data {
        Data(const std::string& name, const std::string& description, Type type)
            : Item::Data(name, description, Item::Type::WEAPON, false), type(type) {}

        Type type;
    };

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

protected:
    Weapon(Data* data, Entity entity) : Item(data, entity) {}
};

}  // namespace wheel
