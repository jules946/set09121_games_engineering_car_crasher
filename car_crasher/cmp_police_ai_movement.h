// cmp_police_ai_movement.h
#pragma once
#include "cmp_actor_movement.h"
#include "ecm.h"
#include "entity_manager.h"


class PolicePursuitComponent final : public ActorMovementComponent {
private:
    EntityManager* _entityManager;
    std::weak_ptr<Entity> _player;
    std::vector<float> _lanePositions;
    int _currentLane;
    bool _isChangingLane;
    float _targetX;
    float _pursuitDistance = 150.0f;
    float _detectionRange = 200.0f;
    float _horizontalSpeed = 250.0f;
    float _decisionDelay = 0.2f;
    float _timeSinceLastDecision = 0.0f;
    float _timeInDisfavoredLane = 0.0f;
    float _maxDisfavoredLaneTime = 1.5f;


    bool isObstacleInLane(float laneX, const EntityManager& em) const;

    int getPlayerLane() const;

    int findBestPursuitLane(const EntityManager& em) const;

    void updatePursuit(double dt);

    float calculateLaneScore(int lane, const EntityManager& em) const;

public:
    PolicePursuitComponent() = delete;
    PolicePursuitComponent(Entity* p, const std::array<float, 4>& lanePositions,
                          const std::shared_ptr<Entity>& player,
                          EntityManager* entityManager);
    void update(double dt) override;
};