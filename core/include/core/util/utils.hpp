#pragma once

#include <rfl.hpp>

namespace core {

class Utils {
public:
    template <typename T>
    static std::string get_type_name() {
        return rfl::internal::remove_namespaces<rfl::internal::get_type_name<T>()>().str();
    }

    static std::string read_file(const std::filesystem::path& path);
};

}  // namespace core
