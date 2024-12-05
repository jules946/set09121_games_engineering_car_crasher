//"cmp_sprite.h"
#pragma once

#include "ecm.h"
#include <SFML/Graphics.hpp>

class ShapeComponent : public Component {
protected:
    std::shared_ptr<sf::Shape> _shape;
public:
    ShapeComponent() = delete;
    explicit ShapeComponent(Entity *p);

    void update(double dt) override;
    void render() override;

    sf::Shape &getShape() const;

    template <typename T, typename... Targs>
    void setShape(Targs... params) {
        _shape.reset(new T(params...));
    }
};


class SpriteComponent : public Component {
protected:
    std::shared_ptr<sf::Sprite> _sprite;
    std::shared_ptr<sf::Texture> _texture;
public:
    SpriteComponent() = delete;
    explicit SpriteComponent(Entity *p);

    void update(double dt) override;
    void render() override;

    sf::Sprite &getSprite() const;

    void setTexture(const std::string &textureFile);
};
