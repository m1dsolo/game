#pragma once

namespace core {

class SaveHandler {
public:
    virtual ~SaveHandler() = default;

    virtual void save() = 0;
    virtual void load() = 0;
};

}  // namespace core
