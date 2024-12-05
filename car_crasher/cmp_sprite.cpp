//"cmp_sprite.cpp"

#include "cmp_sprite.h"
#include "system_renderer.h"

void ShapeComponent::update(double dt) {
    _shape->setPosition(_parent->getPosition());
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p) : Component(p), _shape(std::make_shared<sf::CircleShape>()) {}


SpriteComponent::SpriteComponent(Entity *p)
    : Component(p), _sprite(std::make_shared<sf::Sprite>()) {}

void SpriteComponent::update(double dt) {
    _sprite->setPosition(_parent->getPosition());
}

void SpriteComponent::render() {
    Renderer::queue(_sprite.get());
}

sf::Sprite &SpriteComponent::getSprite() const {
    return *_sprite;
}

void SpriteComponent::setTexture(const std::string &textureFile) {
    _texture = std::make_shared<sf::Texture>();
    if (!_texture->loadFromFile(textureFile)) {
        throw std::runtime_error("Failed to load texture!");
    }
    _sprite->setTexture(*_texture);
}
