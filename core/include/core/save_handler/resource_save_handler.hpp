#pragma once

#include <core/save_handler/save_handler.hpp>
#include <core/global.hpp>
#include <core/util/utils.hpp>

#include <rfl/json.hpp>

namespace core {

template <typename... ExtraResourceTypes>
class ResourceSaveHandler : public SaveHandler {
public:
    void save() override;
    void load() override;

private:
    template <typename... AllResourceTypes>
    rfl::Generic::Object serialize_resources_() {
        rfl::Generic::Object obj;
        (serialize_resource_<AllResourceTypes>(obj), ...);
        return obj;
    }

    template <typename ResourceType>
    void serialize_resource_(rfl::Generic::Object& obj) {
        if (ecs.has_resource<ResourceType>()) {
            obj[Utils::get_type_name<ResourceType>()] = rfl::to_generic(ecs.get_resource<ResourceType>());
        }
    }

    template <typename... AllResourceTypes>
    void deserialize_resources_(const rfl::Generic::Object& obj) {
        (deserialize_resource_<AllResourceTypes>(obj), ...);
    }

    template <typename ResourceType>
    void deserialize_resource_(const rfl::Generic::Object& obj) {
        if (auto res = obj.get(Utils::get_type_name<ResourceType>()); res.has_value()) {
            ecs.add_resource(rfl::from_generic<ResourceType>(res.value()).value());
        }
    }
};

template <typename... ExtraResourceTypes>
void ResourceSaveHandler<ExtraResourceTypes...>::save() {
    std::ofstream file("resources.json");
    if (file.is_open()) {
        auto obj = serialize_resources_<
            ExtraResourceTypes...
        >();
        file << rfl::json::write(obj, rfl::json::pretty);
        file.close();
    }
}

template <typename... ExtraResourceTypes>
void ResourceSaveHandler<ExtraResourceTypes...>::load() {
    auto json_content = Utils::read_file("resources.json");
    auto obj = rfl::json::read<rfl::Generic::Object>(json_content).value();
    deserialize_resources_<
        ExtraResourceTypes...
    >(obj);
}

}  // namespace core
