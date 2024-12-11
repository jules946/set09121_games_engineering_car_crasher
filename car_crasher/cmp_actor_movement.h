//"cmp_actor_movement.h"
#pragma once
#include <ecm.h>
#include "game_config.h"
class ActorMovementComponent : public Component {
protected:
    float _speed;

public:
    explicit ActorMovementComponent(Entity* p);
    ActorMovementComponent() = delete;

    float getSpeed() const;
    void setSpeed(float _speed);

    void move(const sf::Vector2f&) const;
    void move(float x, float y) const;

    void render() override {}
    void update(double dt) override;
};


class ObstacleMovementComponent final : public ActorMovementComponent {
protected:
    int _lane;
    static constexpr float baseSpeed = 200.0f;

public:
    explicit ObstacleMovementComponent(Entity* p);
    void update(double dt) override;
    int getLane() const { return _lane; }
    void updateSpeedForDifficulty() {
        float speedMultiplier = isHardDifficulty ? 1.5f : 1.0f;
        setSpeed(baseSpeed * speedMultiplier);
    }
};

