#pragma once

#include <core/layer/game.hpp>

#include <ecs/entity.hpp>

namespace survivor {

class GameLayer : public core::GameLayer {
public:
    void on_attach() override;
    void on_detach() override;
};

}  // namespace survivor
