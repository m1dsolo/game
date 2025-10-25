#include <core/manager/animation_fsm.hpp>
#include <core/config/animation_fsm.hpp>
#include <core/animation_fsm.hpp>

#include <rfl/json.hpp>

#include <filesystem>
#include <ranges>

namespace core {

std::vector<wheel::ID> variant_to_ids(const std::variant<std::string, std::vector<std::string>>& var);

AnimationFSMManager::AnimationFSMManager() {
    for (const auto& entry : std::filesystem::recursive_directory_iterator("assets/animation_fsm")) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "[begin load animation fsm...]" << entry.path() << std::endl;
                const auto [name, transitions] = rfl::json::read<AnimationFSMConfig>(file).value();
                wheel::ID id = name;
                AnimationFSM fsm{id};
                for (const auto& [from, to, condition] : transitions) {
                    auto from_state_ids = variant_to_ids(from);
                    auto to_state_ids = variant_to_ids(to);
                    auto condition_ids = variant_to_ids(condition);
                    for (const auto from_state_id : from_state_ids) {
                        for (const auto to_state_id : to_state_ids) {
                            fsm.transitions[from_state_id].emplace_back(to_state_id, condition_ids);
                        }
                    }
                }
                set(id, std::move(fsm));
                std::cout << "[end load animation fsm...]" << std::endl;
            }
        }
    }
}

std::vector<wheel::ID> variant_to_ids(const std::variant<std::string, std::vector<std::string>>& var) {
    return std::visit(
        [](auto&& arg) -> std::vector<wheel::ID> {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return {arg};
            } else {
                return std::views::transform(arg, [](const std::string& s) { return wheel::ID{s}; }) |
                    std::ranges::to<std::vector<wheel::ID>>();
            }
        },
        var
    );
}

}  // namespace core
