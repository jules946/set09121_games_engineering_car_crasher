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

class PlayerMovementComponent final : public ActorMovementComponent {
public:
    explicit PlayerMovementComponent(Entity* p);
    void update(double dt) override;
};


class EnemyAIComponent final : public ActorMovementComponent {
private:
    sf::Vector2f _direction;
    sf::Clock _directionChangeTimer;
public:
    explicit EnemyAIComponent(Entity* p);
    void update(double dt) override;
    void changeDirection();

};