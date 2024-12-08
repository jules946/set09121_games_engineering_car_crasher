#include "obstacle_manager.h"
#include "cmp_actor_movement.h"
#include "cmp_sprite.h"
#include "game_config.h"

ObstacleManager::ObstacleManager(EntityManager& entityManager)
    : _entityManager(entityManager),
      _spawnInterval(2.0f) {
    _spawnClock.restart();
}

void ObstacleManager::addObstacleSprite(const std::string& spritePath) {
    _obstacleSprites.push_back(spritePath);
}

const std::string& ObstacleManager::getRandomSprite() const {
    return _obstacleSprites[rand() % _obstacleSprites.size()];
}

void ObstacleManager::update(double dt) {

    // Debug print to check spawn timing
    std::cout << "Time since last spawn: " << _spawnClock.getElapsedTime().asSeconds() << std::endl;

    if (_spawnClock.getElapsedTime().asSeconds() > _spawnInterval) {
        if (auto obstacle = createObstacle()) {
            std::cout << "Created new obstacle" << std::endl;
            _entityManager.list.push_back(obstacle);
        } else {
            std::cout << "Failed to create obstacle" << std::endl;
        }
        _spawnClock.restart();
    }
}

std::shared_ptr<Entity> ObstacleManager::createObstacle() {
    if (_obstacleSprites.empty()) return nullptr;

    auto obstacle = std::make_shared<Entity>();

    // Add sprite component
    auto sprite = obstacle->addComponent<SpriteComponent>();
    sprite->setTexture(getRandomSprite());
    sprite->getSprite().setScale(2.0f, 2.0f);
    sprite->getSprite().setOrigin(
        sprite->getSprite().getLocalBounds().width / 2.f,
        sprite->getSprite().getLocalBounds().height / 2.f
    );

    // Add movement component
    auto movement = obstacle->addComponent<ObstacleMovementComponent>();

    // Set initial position using game_config lane positions
    int lane = movement->getLane();
    obstacle->setPosition(sf::Vector2f(lanePositions[lane], -50.f));

    return obstacle;
}