#include <core/manager/save.hpp>

namespace core {

void SaveManager::save() {
    if (!entity_save_handler_) {
        register_components();
    }
    entity_save_handler_->save();

    if (!resource_save_handler_) {
        register_resources();
    }
    resource_save_handler_->save();
}

void SaveManager::load() {
    if (!entity_save_handler_) {
        register_components();
    }
    entity_save_handler_->load();

    if (!resource_save_handler_) {
        register_resources();
    }
    resource_save_handler_->load();
}

}  // namespace core
