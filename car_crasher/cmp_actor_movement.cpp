// "cmp_actor_movement.cpp"
#include "cmp_actor_movement.h"
#include "game_config.h"  // Include this for gameHeight

using namespace sf;

void ActorMovementComponent::update(double dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
    : Component(p), _speed(100.0f) {}

ObstacleMovementComponent::ObstacleMovementComponent(Entity* p)
    : ActorMovementComponent(p) {
    _lane = rand() % numLanes;  // Use numLanes from game_config.h
    setSpeed(200.0f);
}

void ObstacleMovementComponent::update(double dt) {
    _parent->setPosition(sf::Vector2f(lanePositions[_lane],
                                    _parent->getPosition().y + getSpeed() * dt));

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




