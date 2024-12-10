//game_UI_Manager.cpp
#include "cmp_sprite.h"
#include "car_crasher.h"
#include "game_config.h"
#include "game_UI_Manager.h"
#include <SFML/Graphics.hpp>


int livesInt = 3;

void gameUIManager::loadLives(EntityManager& entityManager, int livesInt) {
  // TODO add heart image
    if (!livesTexture.loadFromFile("res/img/heartLife.png")) {
        throw std::runtime_error("Failed to load road texture!");
    }

    //const sf::Vector2f textureSize(livesTexture.getSize().x, livesTexture.getSize().y);
    //const float scaleX = columnWidth / textureSize.x;
    //const float scaledWidth = textureSize.x * scaleX;

    for (int i = 0; i < livesInt; i++) {
        auto heart = std::make_shared<Entity>();

        float yPos = 20.f;
        float xPos = gameWidth - 150.f - i*45;

        heart->setPosition(sf::Vector2f(xPos, yPos));

        auto s = heart->addComponent<SpriteComponent>();
        s->getSprite().setTexture(livesTexture);
        s->getSprite().setScale(0.1f, 0.1f);
        s->getSprite().setOrigin(0, 0);
        //s->getSprite().setPosition(sf::Vector2f(xPos, yPos));
        
        entityManager.list.push_back(heart);
    }
}


void gameUIManager::update(double dt, EntityManager& entityManager, int livesInt) {
    auto& entities = entityManager.list;
    std::vector<std::shared_ptr<Entity>> uiHearts;

    // Collect UI hearts
    for (const auto& entity : entities) {
        if (entity->getPosition().y == 20.f) {
            uiHearts.push_back(entity);
        }
    }

    // Remove excess hearts
    while (uiHearts.size() > livesInt) {
        uiHearts.back()->setForDelete();
        uiHearts.pop_back();
    }

    // Add missing hearts
    while (uiHearts.size() < livesInt) {
        auto heart = std::make_shared<Entity>();
        float yPos = 20.f;
        float xPos = gameWidth - 150.f - uiHearts.size() * 45;
        heart->setPosition(sf::Vector2f(xPos, yPos));

        auto s = heart->addComponent<SpriteComponent>();
        s->getSprite().setTexture(livesTexture);
        s->getSprite().setScale(0.1f, 0.1f);
        s->getSprite().setOrigin(0, 0);

        entityManager.list.push_back(heart);
        uiHearts.push_back(heart);
    }
}


/*
void gameUIManager::update(double dt, EntityManager& entityManager, int livesInt) {
    auto& entities = entityManager.list;
    for (const auto& entity : entities) {
        //auto heartEntity = entity->getPosition();
        if (livesInt < 3 && entity->getPosition() == sf::Vector2f(gameWidth - 150.f - livesInt * 45, 20.f)) {
            entity->setForDelete();
        }
    }
}
*/