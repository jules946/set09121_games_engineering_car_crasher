//car_crasher.cpp
#include <cmath>

#include "car_crasher.h"
#include "cmp_player_movement.h"
#include "game_config.h"
#include "cmp_sound_effect.h"
#include "ecm.h"
#include "cmp_sprite.h"
#include "system_renderer.h"
#include "obstacles.h"
#include "background_manager.h"

using namespace sf;
using namespace std;

std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> menuScene;
std::shared_ptr<Scene> gameScene;

constexpr float GameScene::_lanePositions[3];

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
    // text.setString("Car Crasher");
}

void MenuScene::render() {
    Renderer::queue(&text);
    Scene::render();
}

// GameScene class implementation
void GameScene::load() {
    // add background
    _backgroundManager.loadBackground(_entity_manager);

    // Create player
    const auto player = make_shared<Entity>();

    // Add sprite component first
    const auto s = player->addComponent<SpriteComponent>();
    s->setTexture("res/img/BlueCar.png");
    s->getSprite().setScale(2.0f, 2.0f);
    s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2.f,
                             s->getSprite().getLocalBounds().height / 2.f);

    // Set initial position to middle lane
    player->setPosition(Vector2f(lanePositions[1], gameHeight / 2.f));

    // Add components
    player->addComponent<SoundEffectComponent>("res/sound/tires_squal_loop.wav");
    player->addComponent<PlayerMovementComponent>(lanePositions);

    _entity_manager.list.push_back(player);
}

void GameScene::update(const double dt) {
    if (Keyboard::isKeyPressed(Keyboard::Tab)) {
        activeScene = menuScene;
    }

    // Spawn new obstacle every spawnInterval seconds
    if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
        auto obstacle = Obstacle::makeObstacle(_lanePositions);
        _entity_manager.list.push_back(obstacle);
        spawnClock.restart();
    }

    Scene::update(dt);
}

// Add all entities to the renderer queue
void GameScene::render() {
    Scene::render();
}
