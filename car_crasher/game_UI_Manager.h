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
    sf::Text pauseText;

public:
    void loadLives(EntityManager& entityManager, int livesInt);
    void update(double dt, EntityManager& entityManager);
    void resetLives() { livesInt = 3; } // Reset hearts when quitting the game
    void loadLivesText(Font& font, Text& livesText);
    void loadScoreText(Font& font, Text& scoreText);
    void loadGameOverText(Font& font, Text& gameOverText, Text& gameOverScoreText, Text& promptText);
    void loadPauseText(Font& font, Text& pauseText);
};


#endif // GAME_UI_MANAGER_H
