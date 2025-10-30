#pragma once

#include <core/save_handler/save_handler.hpp>
#include <core/save_handler/entity_save_handler.hpp>
#include <core/save_handler/resource_save_handler.hpp>

#include <wheel/singleton.hpp>

#include <memory>

namespace core {

struct NameComponent;
struct HPComponent;

class SaveManager : public wheel::Singleton<SaveManager> {
    friend wheel::Singleton<SaveManager>;

public:
    template <typename... ComponentTypes>
    void register_components() {
        entity_save_handler_ = std::make_unique<EntitySaveHandler<
            ComponentTypes...
        >>();
    }

    template <typename... ResourceTypes>
    void register_resources() {
        resource_save_handler_ = std::make_unique<ResourceSaveHandler<
            ResourceTypes...
        >>();
    }

    void save();

    void load();

private:
    std::unique_ptr<SaveHandler> entity_save_handler_;
    std::unique_ptr<SaveHandler> resource_save_handler_;
};

}  // namespace core
