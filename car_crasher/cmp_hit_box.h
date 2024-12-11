//cmp_hit_box.h
#pragma once

#include <SFML/Graphics.hpp>
#include "ecm.h"

class HitboxComponent final : public Component {
private:
    sf::FloatRect _bounds;

public:
    HitboxComponent(Entity* p, const sf::FloatRect& bounds);

    sf::FloatRect getBounds() const;

    void update(double dt) override;

    void render() override;
};
