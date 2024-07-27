#include <game/resource/config.hpp>

#include <filesystem>

#include <wheel/json.hpp>

namespace wheel {

ConfigResource::ConfigResource() {
    auto json = Json::parse_file(std::filesystem::current_path().string() + "/config.json");
    for (int val : static_cast<JsonListType&>(json["levelup_exps"])) {
        levelup_exps.emplace_back(val);
    }
    fps = json["fps"];
    w = json["w"];
    h = json["h"];
    online = json["online"];
}

ConfigResource::~ConfigResource() {

}

}  // namespace wheel
