// collision_manager.h
#pragma once

#include <memory>
#include "ecm.h"
#include "entity_manager.h"
#include "game_config.h"

class CollisionManager {
public:
    static void checkPlayerCollisions(EntityManager& entityManager, const std::shared_ptr<Entity>& player);

private:
    static void handlePlayerCollision(const std::shared_ptr<Entity>& player, const std::shared_ptr<Entity>& obstacle);
};