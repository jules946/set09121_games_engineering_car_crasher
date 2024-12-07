//game_config.h

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <array>

// Game dimensions
constexpr float gameWidth = 800.0f;  // Example value
constexpr float gameHeight = 600.0f; // Example value

// Lane and column settings
constexpr int totalColumns = 5;
constexpr int numLanes = 3;
constexpr float columnWidth = gameWidth / totalColumns;
constexpr float startX = columnWidth; // First lane starts at the second column

// Calculate lane positions (center of each lane)
constexpr std::array<float, numLanes> lanePositions = {
    startX + columnWidth * 0.5f,
    startX + columnWidth * 1.5f,
    startX + columnWidth * 2.5f
};

// Other shared constants (example)
constexpr float tileScaleY = 2.0f;
constexpr float overlap = 1.0f;

#endif // GAME_CONFIG_H
