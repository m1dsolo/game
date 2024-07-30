#pragma once

#include <vector>
#include <memory>

#include <game/buff.hpp>

#include <game/item/consumable.hpp>

namespace wheel {

class Potion : public Consumable {
public:
    struct Data : Consumable::Data {
        Data(const std::string& name, const std::string& description, int max_uses = 1, const std::vector<std::shared_ptr<Buff>>& buffs = {})
            : Consumable::Data(name, description, Consumable::Type::POTION, false, max_uses), buffs(buffs) {}

        std::vector<std::shared_ptr<Buff>> buffs;
    };

    Potion(Data* data, Entity entity, Slot& slot) : Consumable(data, entity, slot, "drink_potion", []() { return 90; }) {}

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

private:
    void use() override;
};

}  // namespace wheel
