//cmp_bkgrd_movement.h
#pragma once

#include "cmp_actor_movement.h" // Include the parent class header


class BackgroundMovementComponent final : public ActorMovementComponent {
private:
    float _screenHeight; // Height of the screen
    float _tileHeight;     // Height of the background tile

public:
    // Constructor
    explicit BackgroundMovementComponent(Entity* p, float scrollSpeed, float screenHeight, float tileHeight);

    // Override update method to handle scrolling logic
    void update(double dt) override;

    // No rendering logic needed, but override for consistency
    void render() override;
};
