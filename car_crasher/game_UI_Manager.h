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
};


#endif // GAME_UI_MANAGER_H
