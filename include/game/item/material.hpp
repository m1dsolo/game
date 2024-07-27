#pragma once

#include <game/item/item.hpp>

namespace wheel {

class Material : public Item {
public:
    struct Data : Item::Data {
        Data(const std::string& name, const std::string& description)
            : Item::Data(name, description, Item::Type::MATERIAL, true) {}
    };

    Material(Data* data) : Item(data) {}

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }
};

}  // namespace wheel
