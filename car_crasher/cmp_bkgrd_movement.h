//cmp_bkgrd_movement.h
#pragma once

#include "cmp_actor_movement.h"


class BackgroundMovementComponent final : public ActorMovementComponent {
private:
    float _screenHeight;
    float _tileHeight;

public:

    explicit BackgroundMovementComponent(Entity* p, float tileHeight);
    void update(double dt) override;
    void render() override;
};
