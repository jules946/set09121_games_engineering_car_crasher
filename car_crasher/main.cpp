//main.cpp
#include <SFML/Graphics.hpp>
#include <memory>
#include "car_crasher.h"
#include "game_config.h"
#include "system_renderer.h"
#include "scene.h"


using namespace sf;
using namespace std;

void Load() {
    // Load resources and initialize scenes
    gameScene = std::make_shared<GameScene>();
    menuScene = std::make_shared<MenuScene>();
    pauseScene = std::make_shared<PauseScene>();
    gameOverScene = std::make_shared<GameOverScene>();
    keyBindScene = std::make_shared<KeyBindScene>();
    changeCarScene = std::make_shared<ChangeCarScene>();

    gameScene->load();
    menuScene->load();
    pauseScene->load();
    gameOverScene->load();
    keyBindScene->load();
    changeCarScene->load();

    activeScene = menuScene;
}


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
    window.setFramerateLimit(60);
    Renderer::initialise(window);

    try {
        Load(); // Load resources and initialize scenes

        while (window.isOpen()) {
            Event event{};
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close(); // Handle window close event
                }
                if (event.type == Event::Resized) {
                    // Can resize the window (prevents crashes)
                    FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(View(visibleArea));
                }
            }

            window.clear();
            Update();
            Render();
            window.display();
        }
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    Renderer::shutdown();
    return 0;
}