#include <game/system/event.hpp>

#include <game/global.hpp>
#include <game/entity_manager.hpp>

namespace wheel {

void EventSystem::startup() {
    // spawn event
    timer.add(1000000, -1, []() {
        int val = random.uniform(0, 3);
        if (val < 3) {
            EntityManager::instance().create_enemy("skeleton");
        } else {
            EntityManager::instance().create_enemy("goblin");
        }
    });
}

}  // namespace wheel
