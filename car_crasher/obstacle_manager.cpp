//obstacle_manager.cpp
#include "obstacle_manager.h"
#include "cmp_actor_movement.h"
#include "cmp_sprite.h"
#include "game_config.h"
#include "cmp_hit_box.h"

ObstacleManager::ObstacleManager(EntityManager& entityManager)
    : _entityManager(entityManager),
      _spawnInterval(isHardDifficulty ? HARD_SPAWN_INTERVAL : EASY_SPAWN_INTERVAL) {
    _spawnClock.restart();
}

void ObstacleManager::addObstacleSprite(const std::string& spritePath) {
    _obstacleSprites.push_back(spritePath);
}

void ObstacleManager::initializeSprites() {
    // Bad obstacles
    addObstacleSprite("res/img/Construction_sign.png");
    addObstacleSprite("res/img/Street_baracade.png");
    addObstacleSprite("res/img/Street_baracade_2.png");
    addObstacleSprite("res/img/Traffic_cone.png");
    addObstacleSprite("res/img/Taxi.png");
    addObstacleSprite("res/img/Truck.png");
    addObstacleSprite("res/img/School_bus.png");
    addObstacleSprite("res/img/Van_rundown.png");
    // Good obstacles
    addObstacleSprite("res/img/heartLife.png");  // Add heart to sprites
}

const std::string& ObstacleManager::getRandomSprite() const {
    static const std::string heartSprite = "res/img/heartLife.png";

    // Only spawn heart if player has less than max lives and 10% chance
    if (livesInt < 3 && rand() % 10 == 0) {
        return heartSprite;
    }

    // Get random sprite excluding the heart (last sprite)
    return _obstacleSprites[rand() % (_obstacleSprites.size() - 1)];
}


void ObstacleManager::update(double dt) {
    _spawnInterval = isHardDifficulty ? HARD_SPAWN_INTERVAL : EASY_SPAWN_INTERVAL;
    // Debug print to check spawn timing
    // std::cout << "Time since last spawn: " << _spawnClock.getElapsedTime().asSeconds() << std::endl;

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
    auto sprite = obstacle->addComponent<SpriteComponent>();
    std::string spritePath = getRandomSprite();
    sprite->setTexture(spritePath);


    // Set scale based on sprite type
    // Different scale for heart
    if (spritePath == "res/img/heartLife.png") {
        _isGoodObstacle = true;
        sprite->getSprite().setScale(0.1f, 0.1f);
    } else {
        _isGoodObstacle = false;
        sprite->getSprite().setScale(2.0f, 2.0f);
    }

    sprite->getSprite().setOrigin(
        sprite->getSprite().getLocalBounds().width / 2.f,
        sprite->getSprite().getLocalBounds().height / 2.f
    );

    // Add movement component
    auto movement = obstacle->addComponent<ObstacleMovementComponent>();

    // Set initial position using game_config lane positions
    int lane = movement->getLane();
    obstacle->setPosition(sf::Vector2f(lanePositions[lane], -50.f));


    if (spritePath == "res/img/heartLife.png") {
        sprite->getSprite().setScale(0.1f, 0.1f);
        const auto scaledBounds = sprite->getSprite().getGlobalBounds();
        obstacle->addComponent<HitboxComponent>(sf::FloatRect(
            scaledBounds.left,
            scaledBounds.top,
            scaledBounds.width * 0.1f,  // Match sprite scale
            scaledBounds.height * 0.1f
        ));
    } else {
        sprite->getSprite().setScale(2.0f, 2.0f);
        const auto scaledBounds = sprite->getSprite().getGlobalBounds();
        //TODO: Figure out why the hitbox is not matching the sprite perfectly
        obstacle->addComponent<HitboxComponent>(sf::FloatRect(0, 0,
        sprite->getSprite().getLocalBounds().width,
        sprite->getSprite().getLocalBounds().height));
    }
    return obstacle;

}