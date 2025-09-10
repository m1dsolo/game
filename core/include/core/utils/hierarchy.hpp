#pragma once

#include <wheel/singleton.hpp>
#include <ecs/entity.hpp>

#include <vector>
#include <unordered_set>

namespace core {

class Hierarchy : public wheel::Singleton<Hierarchy> {
    friend wheel::Singleton<Hierarchy>;

public:
    static wheel::Entity root();
    static std::vector<wheel::Entity> entities();

    static wheel::Entity detach_entity_from_parent(wheel::Entity entity);
    static std::unordered_set<wheel::Entity> get_all_parents(wheel::Entity entity);
};

}  // namespace core
