//obstacle_manager.h
#pragma once
#include "ecm.h"
#include "entity_manager.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ObstacleManager {
private:
    float _spawnInterval;
    sf::Clock _spawnClock;
    std::vector<std::string> _obstacleSprites;
    EntityManager& _entityManager;
    bool _isGoodObstacle;

public:
    explicit ObstacleManager(EntityManager& entityManager);
    
    void addObstacleSprite(const std::string& spritePath);
    void setSpawnInterval(float interval) { _spawnInterval = interval; }
    void update(double dt);
    void initializeSprites();
    //void addHeartSprite(const std::string& spritePath);

private:
    std::shared_ptr<Entity> createObstacle();
    const std::string& getRandomSprite() const;
};