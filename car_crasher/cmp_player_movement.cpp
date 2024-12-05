//cmp_player_movement.cpp

#include "cmp_player_movement.h"
#include "cmp_sprite.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

PlayerMovementComponent::PlayerMovementComponent(Entity* p, const float* lanePositions, const float moveSpeed,
    const float tiltAngle)
    : ActorMovementComponent(p), _lanePositions(lanePositions), _moveSpeed(moveSpeed), _tiltAngle(tiltAngle) {
    _currentLane = 1; // Start in the middle lane
    _targetX = _lanePositions[_currentLane];
}

void PlayerMovementComponent::update(const double dt) {
    // Input
    // If button for going left is pressed and car is not already moving,
    // and car is not in the left lane (_currentLane > 0).
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && _currentLane > 0 && std::abs(_parent->getPosition().x - _targetX) < 1.0f) {
        // Move to the lane on the left
        _currentLane--;
        // Update the target X position to the new lane
        _targetX = _lanePositions[_currentLane];
    }

    // Check if the right arrow key is pressed, the car is not already moving,
    // and the car is not in the right lane (_currentLane < 2).
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && _currentLane < 2 && std::abs(_parent->getPosition().x - _targetX) < 1.0f) {
        // Move to the lane on the right
        _currentLane++;
        // Update the target X position to the new lane
        _targetX = _lanePositions[_currentLane];
    }

    // Movement between lanes
    // Check if the car is not yet at the target X position (_targetX).
    if (std::abs(_targetX - _parent->getPosition().x) > 1.0f) {
        // Calculate the distance to move based on speed and direction
        const auto moveDistance = static_cast<float>((_targetX - _parent->getPosition().x) * _moveSpeed * dt);

        // Move the car closer to the target X
        move(moveDistance, 0.0f);

        // Add a tilt effect based on the direction of movement
        const float tilt = (_targetX > _parent->getPosition().x) ? _tiltAngle : -_tiltAngle;
        _parent->getComponent<SpriteComponent>()->getSprite().setRotation(tilt);
    } else {
        // Snap to the target lane and reset tilt
        _parent->setPosition(sf::Vector2f(_targetX, _parent->getPosition().y));
        _parent->getComponent<SpriteComponent>()->getSprite().setRotation(0.0f);
    }
}

