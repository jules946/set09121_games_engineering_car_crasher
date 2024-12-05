// "cmp_actor_movement.cpp"
#include "cmp_actor_movement.h"

#include "car_crasher.h"
using namespace sf;

void ActorMovementComponent::update(double dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
    : Component(p), _speed(100.0f) {}


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



