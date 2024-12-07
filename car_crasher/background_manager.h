#ifndef BACKGROUND_MANAGER_H
#define BACKGROUND_MANAGER_H

#include <memory>
#include "entity_manager.h"
#include "SFML/Graphics/Texture.hpp"

class backgroundManager {
private:
    sf::Texture roadTexture; // Store texture persistently

public:
    void loadBackground(EntityManager &entityManager);
};


#endif // BACKGROUND_MANAGER_H
