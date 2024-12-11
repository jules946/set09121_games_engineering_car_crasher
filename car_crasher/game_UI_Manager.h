//game_UI_Manager.h
#ifndef GAME_UI_MANAGER_H
#define GAME_UI_MANAGER_H

#include <memory>
#include "ecm.h"
#include "entity_manager.h"
#include "SFML/Graphics/Texture.hpp"

class gameUIManager {
private:
    sf::Texture livesTexture; // Store texture persistently

public:
    void loadLives(EntityManager& entityManager, int livesInt);
    void update(double dt, EntityManager& entityManager, int livesInt);
    void resetLives() { livesInt = 3; } // Reset hearts when quitting the game
};


#endif // GAME_UI_MANAGER_H
