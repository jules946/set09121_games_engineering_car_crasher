// obstacle_manager.cpp
#include "obstacle_manager.h"

#include <limits>

#include "cmp_sprite.h"
#include "cmp_actor_movement.h"
#include "cmp_hit_box.h"


ObstacleManager::ObstacleManager(EntityManager& entityManager)
    : _entityManager(entityManager),
      _spawnInterval(isHardDifficulty ? HARD_SPAWN_INTERVAL : EASY_SPAWN_INTERVAL),
      lastObstacleYPos{-100.0f, -100.0f, -100.0f, -100.0f} {
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

int ObstacleManager::findBestLane() const {
    float minY = std::numeric_limits<float>::infinity();  // Changed from max to min
    int bestLane = 0;

    // Debug output
    std::cout << "Lane positions: ";
    for (int lane = 0; lane < numLanes; ++lane) {
        std::cout << lastObstacleYPos[lane] << " ";
    }
    std::cout << std::endl;

    for (int lane = 0; lane < numLanes; ++lane) {
        float spaceInLane = lastObstacleYPos[lane];
        if (spaceInLane < minY) {  // Changed from > to
            minY = spaceInLane;
            bestLane = lane;
        }
    }

    std::cout << "Selected lane: " << bestLane << " with Y: " << minY << std::endl;
    return bestLane;
}

void ObstacleManager::updateLaneTracking() {
    // Reset tracking
    for (int i = 0; i < numLanes; ++i) {
        lastObstacleYPos[i] = -100.0f;
    }

    // Update with current obstacle positions
    for (const auto& entity : _entityManager.list) {
        if (auto movement = entity->getComponent<ObstacleMovementComponent>()) {
            int lane = movement->getLane();
            float yPos = entity->getPosition().y;
            lastObstacleYPos[lane] = std::max(lastObstacleYPos[lane], yPos);
            std::cout << "Updating lane " << lane << " with Y: " << yPos << std::endl;
        }
    }
}

std::shared_ptr<Entity> ObstacleManager::createObstacle() {
    if (_obstacleSprites.empty()) return nullptr;

    updateLaneTracking();

    auto obstacle = std::make_shared<Entity>();
    auto sprite = obstacle->addComponent<SpriteComponent>();
    std::string spritePath = getRandomSprite();
    sprite->setTexture(spritePath);

    // Determine the type of obstacle
    bool isCarObstacle = (spritePath == "res/img/Taxi.png" ||
                          spritePath == "res/img/Truck.png" ||
                          spritePath == "res/img/School_bus.png" ||
                          spritePath == "res/img/Van_rundown.png");

    // Set scale based on sprite type
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

    const int bestLane = findBestLane();
    float carObstacleSpeedPercentage = 0.5f;
    float obstacleSpeed = isCarObstacle
                              ? baseSpeed * carObstacleSpeedPercentage  // Car obstacles are slower
                              : baseSpeed;                              // Non-car obstacles use baseSpeed

    auto movement = obstacle->addComponent<ObstacleMovementComponent>(bestLane, obstacleSpeed);
    obstacle->setPosition(sf::Vector2f(lanePositions[bestLane], -50.f));

    // Add hitbox
    if (_isGoodObstacle) {
        const auto scaledBounds = sprite->getSprite().getGlobalBounds();
        obstacle->addComponent<HitboxComponent>(sf::FloatRect(
            scaledBounds.left, scaledBounds.top,
            scaledBounds.width * 0.1f, scaledBounds.height * 0.1f
        ));
    } else {
        obstacle->addComponent<HitboxComponent>(sf::FloatRect(0, 0,
            sprite->getSprite().getLocalBounds().width,
            sprite->getSprite().getLocalBounds().height));
    }

    return obstacle;
}

void ObstacleManager::update(double dt) {
    _spawnInterval = isHardDifficulty ? HARD_SPAWN_INTERVAL : EASY_SPAWN_INTERVAL;

    if (_spawnClock.getElapsedTime().asSeconds() > _spawnInterval) {
        if (auto obstacle = createObstacle()) {
            // std::cout << "Created new obstacle" << std::endl;
            _entityManager.list.push_back(obstacle);
        } else {
            // std::cout << "Failed to create obstacle" << std::endl;
        }
        _spawnClock.restart();
    }

    // Cleanup obstacles that are off-screen
    _entityManager.list.erase(
        std::remove_if(
            _entityManager.list.begin(),
            _entityManager.list.end(),
            [](const std::shared_ptr<Entity>& entity) {
                return entity->is_fordeletion();
            }
        ),
        _entityManager.list.end()
    );
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