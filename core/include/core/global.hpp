#pragma once

#include <core/resource/config.hpp>
#include <core/resource/context.hpp>

#include <ecs/ecs.hpp>

namespace core {

inline wheel::ECS ecs;
inline ConfigResource config;
inline ContextResource context;

}  // namespace core
