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
            : Consumable::Data(name, description, max_uses), buffs(buffs) {}

        std::vector<std::shared_ptr<Buff>> buffs;
    };

    Potion(Data* data, Entity entity, Slot& slot);

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

    virtual std::string info() const override {
        auto& d = data();
        return std::format("{}: {}/{}", d.name, uses_, data().max_uses);
    }

private:
    void on_effect();
};

}  // namespace wheel
