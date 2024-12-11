//game_UI_Manager.h
#ifndef GAME_UI_MANAGER_H
#define GAME_UI_MANAGER_H

#include "entity_manager.h"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

class gameUIManager {
private:
    Texture livesTexture; // Store texture persistently
    Font _font;
    Text pauseText;

public:
    void loadFont(const Font& font);
    void loadLives(EntityManager& entityManager, int livesInt);
    void setupTexts(Text &scoreText, Text &livesText, Text &pauseText) const;
    static void updateScoreText(Text &scoreText);
    void update(double dt, EntityManager &entityManager, Text &scoreText) const;
    void setupGameOverTexts(Text &gameOverText, Text &gameOverScoreText, Text &promptText) const;
    static void updateGameOverScore(Text &gameOverScoreText);
};


#endif // GAME_UI_MANAGER_H
