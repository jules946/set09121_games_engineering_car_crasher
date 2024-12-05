//cmp_player_movement.h

#pragma once
#include "cmp_actor_movement.h"

class PlayerMovementComponent final : public ActorMovementComponent {
protected:
    int _currentLane; // Index of the current lane (0, 1, 2)
    const float* _lanePositions; // Pointer to the array of lane X positions
    float _targetX; // Target X position for lane switching
    float _moveSpeed; // Speed of the animation for lane switching
    float _tiltAngle; // Maximum tilt angle for visual effects

public:
    explicit PlayerMovementComponent(Entity* p, const float* lanePositions, float moveSpeed = 10.0f, float tiltAngle = 15.0f);

    void update(double dt) override;
};
