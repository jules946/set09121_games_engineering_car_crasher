//cmp_bkgrd_movement.cpp
#include "cmp_bkgrd_movement.h"
#include "car_crasher.h"


// Constructor
BackgroundMovementComponent::BackgroundMovementComponent(Entity* p, float scrollSpeed, float screenHeight, float tileHeight)
    : ActorMovementComponent(p), _screenHeight(screenHeight), _tileHeight(tileHeight) {
    setSpeed(scrollSpeed); // Use inherited speed functionality
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

// Render method
void BackgroundMovementComponent::render() {
    // No rendering logic needed
}

