#include <core/util/utils.hpp>

#include <fstream>

namespace core {

std::string Utils::read_file(const std::filesystem::path& path) {
    std::ifstream file(path);
    std::string content;
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        content.resize(file_size);
        file.seekg(0, std::ios::beg);
        file.read(&content[0], file_size);
        file.close();
    }
    return content;
}

}  // namespace core
