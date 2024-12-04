// "cmp_actor_movement.cpp"
#include "cmp_actor_movement.h"
#include <cstdlib>
#include <SFML/Window/Keyboard.hpp>

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


PlayerMovementComponent::PlayerMovementComponent(Entity* p)
    : ActorMovementComponent(p) {}

void PlayerMovementComponent::update(const double dt) {
    if (Keyboard::isKeyPressed(Keyboard::W)) {
        move(0, -_speed * dt);
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        move(0, _speed * dt);
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        move(-_speed * dt, 0);
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        move(_speed * dt, 0);
    }
}


EnemyAIComponent::EnemyAIComponent(Entity* p)
    : ActorMovementComponent(p) {}

void EnemyAIComponent::update(double dt) {
    // Move the ghost in the current direction
    move(_direction * _speed * static_cast<float>(dt));

    // Check if 2 seconds have elapsed
    if (_directionChangeTimer.getElapsedTime().asSeconds() >= 2.0f) {
        changeDirection();
        _directionChangeTimer.restart(); // Reset the timer
    }

        // Keep the ghost within the screen bounds
    if (_parent->getPosition().x < 0 || _parent->getPosition().x > gameWidth||
        _parent->getPosition().y < 0 || _parent->getPosition().y > gameHeight) {
        changeDirection();
        }
    }


// Method to change direction randomly
void EnemyAIComponent::changeDirection() {
    _direction.x = static_cast<float>((rand() % 3) - 1); // Random value between -1 and 1
    _direction.y = static_cast<float>((rand() % 3) - 1); // Random value between -1 and 1
    if (_direction.x == 0 && _direction.y == 0) {
        _direction.x = 1; // Ensure the ghost is always moving
    }
}


