//main.cpp
#include <SFML/Graphics.hpp>
#include <memory>
#include "car_crasher.h"
#include "game_config.h"
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

    try {
        Load(); // Load resources and initialize scenes

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    window.close();
                }
                if (event.type == Event::Closed) {
                    window.close(); // Handle window close event
                }
                if (event.type == Event::Resized) {
                    // Adjust view to fit resized window
                    FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(View(visibleArea));
                }
            }

            window.clear(); // Clear the window
            Update();       // Update game logic
            Render();       // Render current scene
            window.display();
        }
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    Renderer::shutdown(); // Clean up renderer
    return 0;
}