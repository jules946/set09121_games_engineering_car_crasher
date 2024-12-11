// "cmp_actor_movement.cpp"
#include "cmp_actor_movement.h"
#include "game_config.h"

using namespace sf;

void ActorMovementComponent::update(double dt) {}

// ActorMovementComponent constructor
ActorMovementComponent::ActorMovementComponent(Entity* p)
    : Component(p), _speed(100.0f) {}

// ObstacleMovementComponent constructor
ObstacleMovementComponent::ObstacleMovementComponent(Entity* p, const int bestLane, const float speed)
    : ActorMovementComponent(p), _lane(bestLane) {
    const float speedMultiplier = isHardDifficulty ? 1.5f : 1.0f;
    setSpeed(speed * speedMultiplier);
}

//
void ObstacleMovementComponent::update(const double dt) {
    // Move the obstacle down the screen
    _parent->setPosition(sf::Vector2f(lanePositions[_lane],
                                    _parent->getPosition().y + getSpeed() * dt));

    // If the obstacle is off the screen, mark it for deletion
    if (_parent->getPosition().y > gameHeight) {
        _parent->setForDelete();
    }
}

void ActorMovementComponent::move(const Vector2f& p) const {
    const auto newPos = _parent->getPosition() + p;
    _parent->setPosition(newPos);
}

void ActorMovementComponent::move(const float x, const float y) const {
    move(Vector2f(x, y));
}

float ActorMovementComponent::getSpeed() const {
    return _speed;
}

void ActorMovementComponent::setSpeed(const float speed) {
    _speed = speed;
}




