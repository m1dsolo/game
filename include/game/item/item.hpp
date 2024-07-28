#pragma once

#include <unordered_map>

#include <game/global.hpp>
#include <game/action.hpp>

namespace wheel {

class Item {
    friend class Inventory;

public:
    enum class Type {
        NONE,
        MATERIAL,
        WEAPON,
        CONSUMABLE,
    };

    struct Data {
        Data(const std::string& name = "", const std::string& description = "", Type type = Type::NONE, bool stackable = true)
            : name(name), description(description), type(type), stackable(stackable) {}

        std::string name;
        std::string description;
        Type type;
        bool stackable;
        std::unordered_map<std::string, std::shared_ptr<Action>> action_map;
    };

    Item(Data* data = nullptr, Entity entity = EntityNone) : data_(data), entity_(entity) {}
    virtual ~Item() = default;

    void select();
    void unselect();

    virtual Data& data() { return *data_; }
    virtual const Data& data() const { return *data_; }
    Entity entity() const { return entity_; }
    virtual std::string info() const { return data_->name; }

    bool empty() const { return !data_; }

protected:
    Data* data_;
    Entity entity_;
};

}  // namespace wheel
