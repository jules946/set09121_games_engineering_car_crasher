//background.cpp
#include "background.h"
#include "system_renderer.h"
#include "car_crasher.h"  // Add this to get access to gameHeight


void Background::update(double dt) {
    Entity::update(dt);
    if (getPosition().y > gameHeight) {
        setForDelete();
    }
}

std::shared_ptr<Background> Background::makeBackground() {
    auto backG = std::make_shared<Background>();


    // Add sprite based on type
    auto s = backG->addComponent<SpriteComponent>();
    s->setTexture("res/img/background.png");

    // Same scale for all obstacles
    s->getSprite().setScale(1.35f, 1.f);
    s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2.f,
        s->getSprite().getLocalBounds().height / 2.f);

    // Add movement component
    //auto movement = backG->addComponent();

    // Set initial position using the getter
    backG->setPosition(sf::Vector2f(gameWidth/2.f - 200, gameHeight/2.f));

    return backG;
}