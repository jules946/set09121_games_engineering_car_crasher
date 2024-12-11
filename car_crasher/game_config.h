// game_config.h
#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <array>
#include <map>

// Game dimensions
constexpr float gameWidth = 1920.0f;
constexpr float gameHeight = 1080.0f;

// Lane and column settings
constexpr int totalColumns = 6; // Updated to 6 total columns
constexpr int numLanes = 4;     // 4 playable lanes
constexpr float columnWidth = gameWidth / totalColumns;
constexpr float startX = columnWidth; // First lane starts at the second column

// Calculate lane positions (center of each lane for 4 lanes)
constexpr std::array<float, numLanes> lanePositions = {
    startX + columnWidth * 0.5f, // Lane 1
    startX + columnWidth * 1.5f, // Lane 2
    startX + columnWidth * 2.5f, // Lane 3
    startX + columnWidth * 3.5f  // Lane 4
};

// Base speed for non-car obstacles and background
constexpr float baseSpeed = 200.0f;

// Other shared constants (e.g., tile size, overlap)
constexpr float tileScaleY = 2.0f;
constexpr float overlap = 1.0f;

// Game state
extern int livesInt;
extern int score;

// Car configuration
extern const std::map<std::string, std::string> CAR_CONFIGS;
extern std::string selectedCar;

// Difficulty
extern bool isHardDifficulty;
#endif
