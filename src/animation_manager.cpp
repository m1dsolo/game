#include <game/animation_manager.hpp>

#include <game/global.hpp>
#include <game/utils.hpp>

namespace wheel {

Animations* AnimationManager::get_animations(const std::string& name) {
    return animations_map_.at(name).get();
}

AnimationManager::AnimationManager() {
    const std::string& path = std::format("{}/resources/animation", game_resource.path);
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        const std::string& name = entry.path().stem().string();
        animations_map_[name] = std::make_shared<Animations>(name);
        Log::debug("AnimationManager: load animation {}", entry.path().stem().string());
    }
}

}  // namespace wheel
