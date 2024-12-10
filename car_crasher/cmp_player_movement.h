// cmp_player_movement.h
#pragma once
#include "cmp_actor_movement.h"
#include "cmp_sound_effect.h"
#include "game_config.h"
#include <array>

class PlayerMovementComponent final : public ActorMovementComponent {
protected:
    int _currentLane;
    std::array<float, numLanes> _lanePositions;
    float _targetX;
    float _moveSpeed;
    float _tiltAngle;
    std::shared_ptr<SoundEffectComponent> _soundEffect;

public:
    explicit PlayerMovementComponent(Entity* p, const std::array<float, numLanes>& lanes,
                                   float moveSpeed = 10.0f, float tiltAngle = 15.0f);

    void update(double dt) override;

};