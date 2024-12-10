//game_UI_Manager.h
#ifndef GAME_UI_MANAGER_H
#define GAME_UI_MANAGER_H

#include <memory>
#include "ecm.h"
#include "entity_manager.h"
#include "SFML/Graphics/Texture.hpp"

using namespace sf;

class gameUIManager {
private:
    sf::Texture livesTexture; // Store texture persistently

public:
    void loadLives(EntityManager& entityManager, int livesInt);
    void update(double dt, EntityManager& entityManager);
    void resetLives() { livesInt = 3; } // Reset hearts when quitting the game
    void gameUIManager::loadLivesText(Font& font, Text& livesText);
    void gameUIManager::loadScoreText(Font& font, Text& scoreText);
    void gameUIManager::loadGameOverText(Font& font, Text& gameOverText, Text& gameOverScoreText, Text& promptText);
};


#endif // GAME_UI_MANAGER_H
