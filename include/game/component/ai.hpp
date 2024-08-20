#pragma once

#include <game/behavior_tree.hpp>

namespace wheel {

struct AIComponent {
    BehaviorTree* behavior_tree;
};

struct TrackNearestPlayerTag {};
struct AStarTrackNearestPlayerTag {};

}  // namespace wheel
