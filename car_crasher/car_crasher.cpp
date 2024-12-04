//car_crasher.cpp
#include "car_crasher.h"

#include <cmath>

#include "cmp_actor_movement.h"
#include "ecm.h"
#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace sf;
using namespace std;

std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> menuScene;
std::shared_ptr<Scene> gameScene;

#define GHOSTS_COUNT 4

// MenuScene class implementation
void MenuScene::load() {
    if (!font.loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }
    text.setFont(font); // Set the font of the text
    text.setCharacterSize(24); // Set the character size
    text.setFillColor(sf::Color::White); // Set the text color
    text.setString("Almost Pacman");
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
    text.setString("Almost Pacman");
}

void MenuScene::render() {
    Renderer::queue(&text);
    Scene::render();
}

// GameScene class implementation
void GameScene::load() {
    const auto pl = make_shared<Entity>();

    const auto s = pl->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(12.f);
        s->getShape().setFillColor(Color::Yellow);
        s->getShape().setOrigin(Vector2f(12.f, 12.f));
        pl->setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));
        pl->addComponent<PlayerMovementComponent>();
        _ents.list.push_back(pl);

    const Color ghost_cols[]{{208, 62, 25},         // red Blinky
                                 {219, 133, 28},    // orange Clyde
                                 {70, 191, 238},    // cyan Inky
                                 {234, 130, 229}};  // pink Pinky

    for (int i = 0; i < GHOSTS_COUNT; ++i) {
        auto ghost = make_shared<Entity>();
        auto s = ghost->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(12.f);
        s->getShape().setFillColor(ghost_cols[i % 4]);
        s->getShape().setOrigin(Vector2f(12.f, 12.f));
        ghost->addComponent<EnemyAIComponent>();

        _ents.list.push_back(ghost);
    }
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
