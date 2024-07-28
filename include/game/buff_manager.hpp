#pragma once

#include <string>

#include <wheel/singleton.hpp>

#include <game/buff.hpp>

namespace wheel {

// TODO: reflection
class BuffManager : public Singleton<BuffManager> {
    friend class Singleton<BuffManager>;

public:
    template <typename... Args>
    std::shared_ptr<Buff> create(const std::string& name, Args&&... args) {
        if (name == "hp") {
            return std::make_shared<HPBuff>(std::forward<Args>(args)...);
        }
        return nullptr;
    }

private:
    BuffManager() = default;
    ~BuffManager() = default;
    BuffManager(const BuffManager&) = delete;
};

}  // namespace wheel
