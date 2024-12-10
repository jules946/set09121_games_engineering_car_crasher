// cmp_police_movement.cpp
#include "cmp_police_ai_movement.h"
#include "cmp_sprite.h"

#include <array>

#include "entity_manager.h"

PolicePursuitComponent::PolicePursuitComponent(Entity* p, const std::array<float, 4>& lanePositions,
                                               const std::shared_ptr<Entity>& player,
                                               EntityManager* entityManager)
   : ActorMovementComponent(p), _entityManager(entityManager), _player(player),
     _lanePositions(std::vector<float>(lanePositions.begin(), lanePositions.end())),
     _currentLane(1), _isChangingLane(false), _timeSinceLastDecision(999.0f) { // Force immediate decision
    _targetX = _lanePositions[_currentLane];
    setSpeed(200.0f);
}

bool PolicePursuitComponent::isObstacleInLane(const float laneX, const EntityManager& em) const {
    const auto pos = _parent->getPosition();

    for (const auto& entity : em.list) {
        // Skip if it's the police car itself, the player, or marked for deletion
        if (entity.get() == _parent || entity == _player.lock() || entity->is_fordeletion())
            continue;

        const auto entityPos = entity->getPosition();

        // Check if obstacle is in the lane and ahead of police
        if (std::abs(entityPos.x - laneX) < 20.0f && // Same lane
            entityPos.y < pos.y && // Ahead of police
            pos.y - entityPos.y < _detectionRange) { // Within detection range
            return true;
        }
    }
    return false;
}

float PolicePursuitComponent::calculateLaneScore(int lane, const EntityManager& em) const {
    float score = 100.0f;
    const auto playerPtr = _player.lock();
    if (!playerPtr) return 0.0f;

    float laneX = _lanePositions[lane];
    auto policePos = _parent->getPosition();
    auto playerPos = playerPtr->getPosition();

    // Decrease score if there are obstacles in this lane
    if (isObstacleInLane(laneX, em)) {
        score -= 80.0f;
    }

    // Decrease score based on distance from player's lane
    float distanceFromPlayer = std::abs(laneX - playerPos.x);
    score -= (distanceFromPlayer * 0.5f);

    // Decrease score if lane change is too drastic
    float currentDistanceFromPlayer = std::abs(policePos.x - playerPos.x);
    if (std::abs(laneX - policePos.x) > currentDistanceFromPlayer) {
        score -= 20.0f;
    }

    return score;
}

int PolicePursuitComponent::findBestPursuitLane(const EntityManager& em) const {
    int bestLane = _currentLane;
    float bestScore = calculateLaneScore(_currentLane, em);

    for (int i = 0; i < _lanePositions.size(); ++i) {
        if (i == _currentLane) continue;

        const float score = calculateLaneScore(i, em);
        if (score > bestScore) {
            bestScore = score;
            bestLane = i;
        }
    }

    return bestLane;
}

void PolicePursuitComponent::updatePursuit(double dt) {
    const auto playerPtr = _player.lock();
    if (!playerPtr) return;

    auto pos = _parent->getPosition();
    auto playerPos = playerPtr->getPosition();

    // Update horizontal position (lane changing)
    if (_isChangingLane) {
        float dx = _targetX - pos.x;
        float moveAmount = _speed * dt;

        if (std::abs(dx) <= moveAmount) {
            pos.x = _targetX;
            _isChangingLane = false;
        } else {
            pos.x += (dx > 0 ? moveAmount : -moveAmount);
        }
    }

    // Update vertical position (following player)
    float idealDistance = _pursuitDistance;
    float currentDistance = pos.y - playerPos.y;
    float verticalSpeed = _speed;

    // Speed up if too far behind
    if (std::abs(currentDistance) > _pursuitDistance) {
        verticalSpeed *= 1.5f;
    }

    // Move towards ideal pursuit distance
    if (std::abs(currentDistance - idealDistance) > 5.0f) {
        if (currentDistance > idealDistance) {
            pos.y -= verticalSpeed * dt;
        } else {
            pos.y += verticalSpeed * dt;
        }
    }

    _parent->setPosition(pos);
}


void PolicePursuitComponent::update(double dt) {
    const auto playerPtr = _player.lock();
    if (!playerPtr) return;

    updatePursuit(dt);

    auto pos = _parent->getPosition();
    const auto playerPos = playerPtr->getPosition();
    auto sprite = _parent->getComponent<SpriteComponent>();

    // Horizontal movement with tilt
    if (_isChangingLane) {
        const float dx = _targetX - pos.x;
        float moveAmount = _horizontalSpeed * dt;
        if (std::abs(dx) <= moveAmount) {
            pos.x = _targetX;
            _isChangingLane = false;
            // Reset tilt when reached target
            if (sprite) sprite->getSprite().setRotation(0.0f);
        } else {
            move((dx > 0 ? moveAmount : -moveAmount), 0);
            // Apply tilt while moving
            if (sprite) {
                const float tilt = (dx > 0) ? 15.0f : -15.0f;
                sprite->getSprite().setRotation(tilt);
            }
        }
    } else {
        // Reset tilt when not changing lanes
        if (sprite) sprite->getSprite().setRotation(0.0f);
    }

    // Rest of the code remains the same...
    const float idealDistance = _pursuitDistance;
    const float currentDistance = pos.y - playerPos.y;

    if (std::abs(currentDistance - idealDistance) > 5.0f) {
        float verticalMove = 0;
        verticalMove = currentDistance > idealDistance ? -getSpeed() * dt : getSpeed() * dt;
        move(0, verticalMove);
    }

    _timeSinceLastDecision += dt;
    if (_timeSinceLastDecision >= _decisionDelay && !_isChangingLane) {
        _timeSinceLastDecision = 0;
        const int newLane = findBestPursuitLane(*_entityManager);
        if (newLane != _currentLane || isObstacleInLane(_lanePositions[_currentLane], *_entityManager)) {
            _currentLane = newLane;
            _targetX = _lanePositions[_currentLane];
            _isChangingLane = true;
        }
    }
}
