#pragma once

#include <string>
#include <vector>
#include <variant>

namespace core {

class Utils {
public:
    static std::vector<std::string> variant_to_strings(const std::variant<std::string, std::vector<std::string>>& var) {
        return std::visit(
            [](auto&& arg) -> std::vector<std::string> {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    return {arg};
                } else {
                    return arg;
                }
            },
            var
        );
    }
};

}  // namespace core
