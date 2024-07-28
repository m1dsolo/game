#pragma once

#include <game/item/weapon.hpp>

#include <format>

namespace wheel {

class Gun: public Weapon {
public:
    struct Data : Weapon::Data {
        Data(const std::string& name, const std::string& description)
            : Weapon::Data(name, description, Weapon::Type::GUN) {}

        int atk;
        int penetration;
        int count;
        int clip;
        int reload_interval;
        int shoot_interval;
        int ammo_speed;
        int angle;
    };

    Gun(Data* data, Entity entity);

    Data& data() override { return *static_cast<Data*>(data_); }
    const Data& data() const override { return *static_cast<Data*>(data_); }

    void shoot();
    void reload();

    int atk() const { return data().atk; }
    int ammo() const { return ammo_; }
    int clip() const { return data().clip; }

    virtual std::string info() const override {
        auto& d = data();
        return std::format("{}: {}/{}", d.name, ammo_, d.clip);
    }

private:
    int ammo_ = 0;
    bool reloading_ = false;
};

}  // namespace wheel
