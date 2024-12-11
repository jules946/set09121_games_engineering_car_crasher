// obstacle_manager.h
#pragma once
#include <ecm.h>
#include <SFML/System/Clock.hpp>
#include <array>
#include <memory>
#include <string>
#include "game_config.h"
#include "entity_manager.h"

class ObstacleManager {
private:
    EntityManager& _entityManager;
    sf::Clock _spawnClock;
    std::vector<std::string> _obstacleSprites;
    bool _isGoodObstacle;

    static constexpr float EASY_SPAWN_INTERVAL = 2.0f;
    static constexpr float HARD_SPAWN_INTERVAL = 0.8f;
    float _spawnInterval;

    // Track obstacles per lane
    std::array<float, numLanes> lastObstacleYPos;

    // Helper functions
    int findBestLane() const;
    void updateLaneTracking();
    const std::string& getRandomSprite() const;

public:
    explicit ObstacleManager(EntityManager& entityManager);
    void addObstacleSprite(const std::string& spritePath);
    void initializeSprites();
    void update(double dt);
    std::shared_ptr<Entity> createObstacle();
};