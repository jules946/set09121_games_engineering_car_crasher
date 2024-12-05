//main.cpp
#include <SFML/Graphics.hpp>
#include <memory>
#include "car_crasher.h"
#include "system_renderer.h"

using namespace sf;
using namespace std;

void Load() {
    // Create menu and game scenes
    menuScene = std::make_shared<MenuScene>();
    gameScene = std::make_shared<GameScene>();

    // load scenes
    menuScene->load();
    gameScene->load();

    // start at main menu
    activeScene = menuScene;
}
// testing github desktop

void Update() {
    static Clock clock;
    const double dt = clock.restart().asSeconds();

    // Update all entities via EntityManager in active scene
    activeScene->update(dt);
}

void Render() {
    activeScene->render();
    // render using renderer system
    Renderer::render();
}


int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "Car Crasher");
    Renderer::initialise(window);
    Load();

    while (window.isOpen()) {
        window.clear();
        Update();
        Render();
        window.display();
    }

    Renderer::shutdown();
    return 0;
}