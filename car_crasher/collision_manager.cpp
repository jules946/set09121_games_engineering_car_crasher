// CollisionSystem.cpp
#include "collision_manager.h"


#include "cmp_hit_box.h"
#include "cmp_sprite.h"

void CollisionManager::checkPlayerCollisions(EntityManager& entityManager, const std::shared_ptr<Entity>& player) {
    // check if player exists
    if (!player) return;

    // check if player hitbox exists
    auto playerHB = player->getComponent<HitboxComponent>();
    if (!playerHB) return;

    auto& entities = entityManager.list;
    for (const auto& entity : entities) {
        if (entity == player) continue; // Skip the player itself

        auto obstacleBB = entity->getComponent<HitboxComponent>();
        if (!obstacleBB) continue;

        // Check for collision between player and obstacle
        if (playerHB->getBounds().intersects(obstacleBB->getBounds())) {
            handlePlayerCollision(player, entity);
        }
    }
}

void CollisionManager::handlePlayerCollision(const std::shared_ptr<Entity>& player, const std::shared_ptr<Entity>& obstacle) {
    auto sprite = obstacle->getComponent<SpriteComponent>();

    if (sprite->getTexturePath() == "res/img/heartLife.png") {
        if (livesInt < 3) {  // Only increment if less than max
            livesInt++;
        }
    } else {
        livesInt--;
    }

    obstacle->setForDelete();

// old code - to keep the to do
/*
    // TODO: Implement collision handling
    std::cout << "Collision detected between player and obstacle!" << std::endl;
    // For now, just remove the obstacle
    obstacle->setForDelete();

    // Decrement livesInt to remove a heart
    livesInt--;
    */
}
