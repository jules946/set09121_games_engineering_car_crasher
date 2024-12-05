//car_crasher.cpp
#include <cmath>

#include "car_crasher.h"
#include "cmp_player_movement.h"
#include "ecm.h"
#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace sf;
using namespace std;

std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> menuScene;
std::shared_ptr<Scene> gameScene;


// MenuScene class implementation
void MenuScene::load() {
    if (!font.loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }
    text.setFont(font); // Set the font of the text
    text.setCharacterSize(24); // Set the character size
    text.setFillColor(sf::Color::White); // Set the text color
    text.setString("Car Crasher");
    // Get text bounds
    const FloatRect bounds = text.getLocalBounds();

    // Ensuring the text's origin and position are aligned to whole pixels to avoid subpixel
    // rendering and prevent blurriness
    text.setOrigin(std::round(bounds.width / 2.f), std::round(bounds.height / 2.f));
    text.setPosition(std::round(gameWidth / 2.f), std::round(gameHeight / 2.f));


}

void MenuScene::update(const double dt) {
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        activeScene = gameScene;
    }
    Scene::update(dt);
    text.setString("Car Crasher");
}

void MenuScene::render() {
    Renderer::queue(&text);
    Scene::render();
}

// GameScene class implementation
void GameScene::load() {
    static constexpr float lanePositions[3] = {100.0f, 200.0f, 300.0f};

    const auto player = make_shared<Entity>();

    // Add Sprite Component
    const auto s = player->addComponent<SpriteComponent>();
    s->setTexture("res/img/BlueCar.png");
    s->getSprite().setScale(2.0f, 2.0f);
    s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2.f, s->getSprite().getLocalBounds().height / 2.f);

    // Add Player Movement Component
    player->addComponent<PlayerMovementComponent>(lanePositions);

    // Set initial position to middle lane and middle of screen height
    player->setPosition(sf::Vector2f(lanePositions[1], gameHeight / 2.f));

    // Add to entity manager
    _entity_manager.list.push_back(player);
}

void GameScene::update(const double dt) {
    if (Keyboard::isKeyPressed(Keyboard::Tab)) {
        activeScene = menuScene;
    }
    Scene::update(dt);
}

// Add all entities to the renderer queue
void GameScene::render() {
    Scene::render();
}
