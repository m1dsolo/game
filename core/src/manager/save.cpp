#include <core/manager/save.hpp>

namespace core {

void SaveManager::save() {
    if (!save_handler_) {
        register_components();
    }
    save_handler_->save();
}

void SaveManager::load() {
    if (!save_handler_) {
        register_components();
    }
    save_handler_->load();
}

}  // namespace core
