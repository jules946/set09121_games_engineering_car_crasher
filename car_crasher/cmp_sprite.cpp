//"cmp_sprite.cpp"

#include "cmp_sprite.h"
#include "system_renderer.h"

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
