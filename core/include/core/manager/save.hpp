#pragma once

#include <core/util/save_handler.hpp>

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
        save_handler_ = std::make_unique<SaveHandler<
            ComponentTypes...
        >>();
    }

    void save();

    void load();

private:
    std::unique_ptr<ISaveHandler> save_handler_;
};

}  // namespace core
