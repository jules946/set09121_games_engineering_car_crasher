//"cmp_actor_movement.h"
#pragma once
#include <ecm.h>
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

public:
    explicit ObstacleMovementComponent(Entity* p, int bestLane, float speed);
    void update(double dt) override;
    int getLane() const { return _lane; }

};

