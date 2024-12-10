//cmp_player_movement.cpp
#include "cmp_key_binds.h"
#include "cmp_player_movement.h"
#include "cmp_sprite.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include "cmp_sound_effect.h"

PlayerMovementComponent::PlayerMovementComponent(Entity* p, const std::array<float, 4>& lanePositions,
                                               float moveSpeed, float tiltAngle)
    : ActorMovementComponent(p), _lanePositions(lanePositions),
      _moveSpeed(moveSpeed), _tiltAngle(tiltAngle) {
    _currentLane = 1; // Start in the middle lane
    _targetX = _lanePositions[_currentLane];
    _soundEffect = _parent->getComponent<SoundEffectComponent>();
}

void PlayerMovementComponent::update(const double dt) {

    if (!_soundEffect) {
        _soundEffect = _parent->getComponent<SoundEffectComponent>();
    }

    if (sf::Keyboard::isKeyPressed(KeyBindComponent::getLeftKey()) && _currentLane > 0 &&
        std::abs(_parent->getPosition().x - _targetX) < 1.0f) {
        _currentLane--;
        _targetX = _lanePositions[_currentLane];
        if (_soundEffect) {
            _soundEffect->playSound();
        }
        }

    if (sf::Keyboard::isKeyPressed(KeyBindComponent::getRightKey()) && _currentLane < numLanes - 1 &&
        std::abs(_parent->getPosition().x - _targetX) < 1.0f) {
        _currentLane++;
        _targetX = _lanePositions[_currentLane];
        if (_soundEffect) {
            _soundEffect->playSound();
        }
        }

    if (std::abs(_targetX - _parent->getPosition().x) > 1.0f) {
        const auto moveDistance = static_cast<float>((_targetX - _parent->getPosition().x) * _moveSpeed * dt);

        move(moveDistance, 0.0f);

        const float tilt = (_targetX > _parent->getPosition().x) ? _tiltAngle : -_tiltAngle;
        _parent->getComponent<SpriteComponent>()->getSprite().setRotation(tilt);
    } else {
        _parent->setPosition(sf::Vector2f(_targetX, _parent->getPosition().y));
        _parent->getComponent<SpriteComponent>()->getSprite().setRotation(0.0f);
        if (_soundEffect) {
            _soundEffect->stopSound();
        }
    }
}

