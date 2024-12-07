//
// Created by jjman on 07/12/2024.
//

#include "obstacle_manager.h"
#include "cmp_actor_movement.h"
#include "cmp_sprite.h"
#include "ecm.h"

using namespace std;
void obstacleManager::createObstacle() {

    // 1. Create new entity
    const auto obstacle = make_shared<Entity>();
    // Add Sprite Component
    const auto s = obstacle->addComponent<SpriteComponent>();
    s->setTexture("res/img/BlueCar.png");
    s->getSprite().setScale(2.0f, 2.0f);
    s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2.f, s->getSprite().getLocalBounds().height / 2.f);
    // 3. add movement component
    obstacle->addComponent<ActorMovementComponent>();
    // 4. set initial position (rng what lane so start in and set it at the top of the screen)
    obstacle->setPosition(sf::Vector2f(lanePositions[1], gameHeight / 2.f));

    // return the entity
}
