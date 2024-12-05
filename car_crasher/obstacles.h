//obstacles.h
#pragma once
#include "ecm.h"
#include "cmp_sprite.h"
#include <SFML/Graphics.hpp>
#include <vector>

// Enum to define different obstacle types
enum class ObstacleType {
    CONSTRUCTION_SIGN,
    STREET_BARACADE,
    STREET_BARACADE_2,
    CONE,
    NUM_TYPES  // Used to get total number of types
};

class ObstacleComponent : public Component {
protected:
    float _speed;
    int _lane;
    const float* _lanePositions;
    ObstacleType _type;

public:
    explicit ObstacleComponent(Entity* p, const float* lanePositions, ObstacleType type);
    void update(double dt) override;
    void render() override {}
    int getLane() const { return _lane; }
};

class Obstacle : public Entity {
private:
    static const std::vector<std::string> OBSTACLE_SPRITES;

public:
    static constexpr float OBSTACLE_SPEED = 200.0f;  // Moved to public
    void update(double dt) override;
    static std::shared_ptr<Obstacle> makeObstacle(const float* lanePositions);
    ~Obstacle() override = default;
};