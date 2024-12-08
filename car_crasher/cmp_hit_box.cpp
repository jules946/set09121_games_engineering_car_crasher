//cmp_hit_box.cpp

#include "cmp_hit_box.h"

HitboxComponent::HitboxComponent(Entity* p, const sf::FloatRect& bounds)
    : Component(p), _bounds(bounds) {}

sf::FloatRect HitboxComponent::getBounds() const {
    return _bounds;
}

void HitboxComponent::update(double dt) {
    // Sync hitbox with entity position
    _bounds.left = _parent->getPosition().x - _bounds.width / 2.f;
    _bounds.top = _parent->getPosition().y - _bounds.height / 2.f;
}

void HitboxComponent::render() {
    // Not needed
}