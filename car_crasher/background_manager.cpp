#include "background_manager.h"
#include "cmp_bkgrd_movement.h"
#include "cmp_sprite.h"
#include "car_crasher.h"
#include "game_config.h"
#include <SFML/Graphics.hpp>

void backgroundManager::loadBackground(EntityManager& entityManager) {
    // Load Road Texture
    if (!roadTexture.loadFromFile("res/img/road2.png")) {
        throw std::runtime_error("Failed to load road texture!");
    }

    // Scale
    const Vector2f textureSize(roadTexture.getSize().x, roadTexture.getSize().y);
    const float scaleX = columnWidth / textureSize.x;
    const float scaledWidth = textureSize.x * scaleX;

    // Create road lanes
    for (int lane = 0; lane < numLanes; ++lane) {
        const float laneX = lanePositions[lane] - (scaledWidth / 2.0f);  // Adjust for center positioning

        for (int y = 0; y < static_cast<int>(std::ceil(gameHeight / (textureSize.y * tileScaleY))) + 2; ++y) {
            auto tile = std::make_shared<Entity>();
            float yPos = y * (textureSize.y * tileScaleY - overlap);
            tile->setPosition(sf::Vector2f(laneX, yPos));

            auto s = tile->addComponent<SpriteComponent>();
            s->getSprite().setTexture(roadTexture);
            s->getSprite().setScale(scaleX, tileScaleY);
            s->getSprite().setOrigin(0, 0);

            // Add movement component for scrolling
            tile->addComponent<BackgroundMovementComponent>(
                textureSize.y * tileScaleY - overlap
            );

            entityManager.list.push_back(tile);
        }
    }
}