//cmp_bkgrd_movement.cpp
#include "cmp_bkgrd_movement.h"
#include "car_crasher.h"
#include "game_config.h"


// Constructor
BackgroundMovementComponent::BackgroundMovementComponent(Entity* p, const float tileHeight)
    : ActorMovementComponent(p), _screenHeight(gameHeight), _tileHeight(tileHeight) {
    // Set speed based on difficulty
    const float speedMultiplier = isHardDifficulty ? 1.5f : 1.0f;
    setSpeed(baseSpeed * speedMultiplier);
}

// Update background position
void BackgroundMovementComponent::update(double dt) {
    // Get current position
    auto pos = _parent->getPosition();

    // Move downward
    pos.y += getSpeed() * static_cast<float>(dt);

    // Wrap around when off screen
    if (pos.y >= _screenHeight) {
        pos.y = -_tileHeight + (pos.y - _screenHeight);
    }

    // Update position
    _parent->setPosition(pos);
}

void BackgroundMovementComponent::render() {
    // No rendering logic needed
}

