//obstacles.cpp
#include "obstacles.h"
#include "game_config.h"
#include "car_crasher.h"  // Add this to get access to gameHeight

// Define sprite paths for each obstacle type
const std::vector<std::string> Obstacle::OBSTACLE_SPRITES = {
    "res/img/Construction_sign.png",
    "res/img/Street_baracade.png",
    "res/img/Street_baracade_2.png",
    "res/img/Traffic_cone.png",
};

ObstacleComponent::ObstacleComponent(Entity* p, const float* lanePositions, ObstacleType type)
    : Component(p), _lanePositions(lanePositions), _type(type) {
    _lane = rand() % 3;  // Random lane selection
    _speed = Obstacle::OBSTACLE_SPEED;  // Same speed for all obstacles
}

void ObstacleComponent::update(double dt) {
    // Move downward while staying in lane
    _parent->setPosition(sf::Vector2f(_lanePositions[_lane],
                                    _parent->getPosition().y + _speed * dt));
}

void Obstacle::update(double dt) {
    Entity::update(dt);
    if (getPosition().y > gameHeight) {
        setForDelete();
    }
}

std::shared_ptr<Obstacle> Obstacle::makeObstacle(const float* lanePositions) {
    auto obstacle = std::make_shared<Obstacle>();

    // Randomly select obstacle type
    ObstacleType type = static_cast<ObstacleType>(rand() % static_cast<int>(ObstacleType::NUM_TYPES));

    // Add sprite based on type
    auto s = obstacle->addComponent<SpriteComponent>();
    s->setTexture(OBSTACLE_SPRITES[static_cast<int>(type)]);

    // Same scale for all obstacles
    s->getSprite().setScale(2.0f, 2.0f);
    s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2.f,
                            s->getSprite().getLocalBounds().height / 2.f);

    // Add movement component
    auto movement = obstacle->addComponent<ObstacleComponent>(lanePositions, type);

    // Set initial position using the getter
    int lane = movement->getLane();
    obstacle->setPosition(sf::Vector2f(lanePositions[lane], -50.0f));

    return obstacle;
}