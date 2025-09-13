#pragma once

#include <wheel/singleton.hpp>

namespace survivor {

class AchievementManager : public wheel::Singleton<AchievementManager> {
    friend wheel::Singleton<AchievementManager>;

public:
    int kill_num() const { return kill_num_; }
    void add_kill(int num = 1) { kill_num_ += num; }

private:
    AchievementManager() = default;
    AchievementManager(const AchievementManager&) = delete;

    int kill_num_ = 0;
};

}  // namespace survivor
