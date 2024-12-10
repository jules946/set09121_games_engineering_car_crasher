//"cmp_sprite.h"
#pragma once

#include "ecm.h"
#include <SFML/Graphics.hpp>

class SpriteComponent final : public Component {
protected:
    std::shared_ptr<sf::Sprite> _sprite;
    std::shared_ptr<sf::Texture> _texture;
    std::string _texturePath;
public:
    SpriteComponent() = delete;
    explicit SpriteComponent(Entity *p);

    void update(double dt) override;
    void render() override;

    sf::Sprite &getSprite() const;
    std::string getTexturePath() const { return _texturePath; }

    void setTexture(const std::string &textureFile);
};
