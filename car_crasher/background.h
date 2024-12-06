//background.h
#pragma once
#include "ecm.h"
#include "cmp_sprite.h"
#include <SFML/Graphics.hpp>
#include <vector>

class BackgroundComponent : public Component {

};

class Background : public Entity {

public:
    static constexpr float BACKGROUND_SPEED = 200.0f;  // Moved to public
    void update(double dt) override;
    static std::shared_ptr<Background> makeBackground();
    ~Background() override = default;
};