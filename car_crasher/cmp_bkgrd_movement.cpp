//cmp_bkgrd_movement.cpp
#include "cmp_bkgrd_movement.h"
#include "car_crasher.h"
#include "game_config.h"


// Constructor
BackgroundMovementComponent::BackgroundMovementComponent(Entity* p, float tileHeight)
    : ActorMovementComponent(p), _screenHeight(gameHeight), _tileHeight(tileHeight) {
    const float speedMultiplier = isHardDifficulty ? 1.5f : 1.0f;
    setSpeed(baseSpeed * speedMultiplier);
}

// Update method
void BackgroundMovementComponent::update(double dt) {
    // Get current position
    auto pos = _parent->getPosition();

    // Move downward
    pos.y += getSpeed() * static_cast<float>(dt);

    // Wrap around when off screen
    // Add a small buffer to ensure smooth transition
    if (pos.y >= _screenHeight) {
        // Calculate exact position to maintain seamless scrolling
        pos.y = -_tileHeight + (pos.y - _screenHeight);
    }

    // Update position
    _parent->setPosition(pos);
}

void BackgroundMovementComponent::render() {
    // No rendering logic needed
}

