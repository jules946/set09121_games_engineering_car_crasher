// cmp_police_ai_movement.cpp
#include "cmp_police_ai_movement.h"
#include "cmp_sprite.h"
#include "entity_manager.h"
#include <array>


// Constructor
PolicePursuitComponent::PolicePursuitComponent(Entity* p,
                                            const std::array<float, 4>& lanePositions,
                                            const std::shared_ptr<Entity>& player,
                                            EntityManager* entityManager)
   : ActorMovementComponent(p),
     _entityManager(entityManager),
     _player(player),
     _lanePositions(lanePositions.begin(), lanePositions.end()),
     _currentLane(1),
     _isChangingLane(false),
     _targetX(_lanePositions[_currentLane]) {}


// Check if there is an obstacle in a given lane
bool PolicePursuitComponent::isObstacleInLane(const float laneX, const EntityManager& em) const {
    const auto pos = _parent->getPosition();

    for (const auto& entity : em.list) {
        // Skip if it's the police car itself, the player, or marked for deletion
        if (entity.get() == _parent || entity == _player.lock() || entity->is_fordeletion())
            continue;

        const auto entityPos = entity->getPosition();

        // Check if obstacle is in the lane and ahead of police car
        if (std::abs(entityPos.x - laneX) < 20.0f && // Same lane
            entityPos.y < pos.y && // Ahead of police
            pos.y - entityPos.y < _detectionRange) { // Within detection range
            return true;
        }
    }
    return false;
}

float PolicePursuitComponent::calculateLaneScore(const int lane, const EntityManager& em) const {
    float score = 100.0f;
    const auto playerPtr = _player.lock();
    if (!playerPtr) return 0.0f;

    const float laneX = _lanePositions[lane];
    const auto playerPos = playerPtr->getPosition();

    // Find player's current lane
    int playerLane = 0;
    for (int i = 0; i < _lanePositions.size(); ++i) {
        if (std::abs(playerPos.x - _lanePositions[i]) < 20.0f) {
            playerLane = i;
            break;
        }
    }

    // Check for obstacles first and give it highest priority
    if (isObstacleInLane(laneX, em)) {
        score -= 300.0f;
    }

    // Heavily penalize same lane as player but less than obstacles (don't want cop car to be constantly behind player)
    else if (lane == playerLane) {
        score -= 200.0f;
    }
    // Favor adjacent lanes
    else if (std::abs(lane - playerLane) == 1) {
        score += 50.0f;
    }
    // Penalize lanes that are too far from player
    else if (std::abs(lane - playerLane) > 1) {
        score -= 100.0f;
    }

    return score;
}

// Find the best lane to pursue the player
int PolicePursuitComponent::findBestPursuitLane(const EntityManager& em) const {
    int bestLane = _currentLane;
    float bestScore = calculateLaneScore(_currentLane, em);

    // Check all lanes except current lane
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

void PolicePursuitComponent::updatePursuit(const double dt) {
    const auto playerPtr = _player.lock();
    if (!playerPtr) return;

    auto pos = _parent->getPosition();
    const auto playerPos = playerPtr->getPosition();

    // Update horizontal position (lane changing)
    if (_isChangingLane) {
        const float dx = _targetX - pos.x;
        float moveAmount = _speed * dt;

        if (std::abs(dx) <= moveAmount) {
            pos.x = _targetX;
            _isChangingLane = false;
        } else {
            pos.x += (dx > 0 ? moveAmount : -moveAmount);
        }
    }

    // Update vertical position (following player)
    const float idealDistance = _pursuitDistance;
    const float currentDistance = pos.y - playerPos.y;
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

    const float idealDistance = _pursuitDistance;
    const float currentDistance = pos.y - playerPos.y;

    // Move towards ideal pursuit distance
    if (std::abs(currentDistance - idealDistance) > 5.0f) {
        float verticalMove = 0;
        verticalMove = currentDistance > idealDistance ? -getSpeed() * dt : getSpeed() * dt;
        move(0, verticalMove);
    }

    // Update decision making
    _timeSinceLastDecision += dt;
    if (_timeSinceLastDecision >= _decisionDelay && !_isChangingLane) {
        _timeSinceLastDecision = 0;
        // Check if a lane change is needed
        const int newLane = findBestPursuitLane(*_entityManager);
        // Change lane if needed
        if (newLane != _currentLane || isObstacleInLane(_lanePositions[_currentLane], *_entityManager)) {
            _currentLane = newLane;
            _targetX = _lanePositions[_currentLane];
            _isChangingLane = true;
        }
    }
}
