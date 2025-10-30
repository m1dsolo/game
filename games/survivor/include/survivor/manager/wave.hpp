#pragma once

#include <ecs/entity.hpp>
#include <wheel/singleton.hpp>
#include <wheel/timer.hpp>

#include <string>
#include <vector>

namespace survivor {

class WaveManager : public wheel::Singleton<WaveManager> {
    friend wheel::Singleton<WaveManager>;

public:
    void start_generate_waves();
    void stop_generate_waves();

    wheel::Entity generate(const std::string& name, const std::vector<wheel::Entity>& boundaries) const;

private:
    WaveManager() = default;
    WaveManager(const WaveManager&) = delete;

    wheel::timer_id_t timer_id_ = 0;
};

}  // namespace survivor
