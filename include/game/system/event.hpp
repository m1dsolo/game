#pragma once

#include <wheel/singleton.hpp>

namespace wheel {

class EventSystem : public Singleton<EventSystem> {
    friend class Singleton<EventSystem>;

public:
    void handle_sdl_event();

    void handle_game_event();

    void handle_tag();

    void startup();

private:
    EventSystem() = default;
    ~EventSystem() = default;
    EventSystem(const EventSystem&) = delete;
};

}  // namespace wheel
