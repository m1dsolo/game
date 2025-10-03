#include <core/manager/animation_fsm.hpp>
#include <core/config/animation_fsm.hpp>
#include <core/util/utils.hpp>
#include <core/animation_fsm.hpp>

#include <rfl/json.hpp>

#include <filesystem>

namespace core {

AnimationFSMManager::AnimationFSMManager() {
    for (const auto& entry : std::filesystem::recursive_directory_iterator("assets/animation_fsm")) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "[begin load animation fsm...]" << entry.path() << std::endl;
                const auto [name, transitions] = rfl::json::read<AnimationFSMConfig>(file).value();
                AnimationFSM fsm{name};
                for (const auto& [from, to, condition] : transitions) {
                    auto froms = Utils::variant_to_strings(from);
                    auto tos = Utils::variant_to_strings(to);
                    auto conditions = Utils::variant_to_strings(condition);
                    // fsm.transitions[std::move(from)].emplace_back(std::move(to), std::move(conditions));
                    for (const auto& f : froms) {
                        for (const auto& t : tos) {
                            fsm.transitions[f].emplace_back(t, conditions);
                        }
                    }
                }
                set(name, std::move(fsm));
                std::cout << "[end load animation fsm...]" << std::endl;
            }
        }
    }
}

}  // namespace core
