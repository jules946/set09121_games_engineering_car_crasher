// CollisionSystem.cpp
#include "collision_manager.h"
#include "cmp_hit_box.h"
#include "cmp_sprite.h"
#include "game_config.h"

void CollisionManager::checkPlayerCollisions(EntityManager& entityManager, const std::shared_ptr<Entity>& player,
    const std::shared_ptr<Entity>& cop) {
    // check if player exists
    if (!player) return;

    // check if player hitbox exists
    const auto playerHB = player->getComponent<HitboxComponent>();
    if (!playerHB) return;

    auto& entities = entityManager.list;
    for (const auto& entity : entities) {
        // Skip player and police car
        if (entity == player || entity == cop) continue;

        const auto obstacleBB = entity->getComponent<HitboxComponent>();
        if (!obstacleBB) continue;

        // check for obstacle between player and obstacle
        if (playerHB->getBounds().intersects(obstacleBB->getBounds())) {
            handlePlayerCollision(player, entity);
        }
    }
}
void CollisionManager::handlePlayerCollision(const std::shared_ptr<Entity>& player, const std::shared_ptr<Entity>& obstacle) {
    const auto sprite = obstacle->getComponent<SpriteComponent>();

    if (sprite->getTexturePath() == "res/img/heartLife.png") {
        if (livesInt < 3) {  // Only increment if less than max
            livesInt++;
        }
    } else {
        livesInt--;
    }

    obstacle->setForDelete();

}
