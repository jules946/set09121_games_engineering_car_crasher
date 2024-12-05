//car_crasher.h
#pragma once

#include <memory>
#include "scene.h"
#include <SFML/Graphics.hpp>

extern std::shared_ptr<Scene> activeScene;
extern std::shared_ptr<Scene> menuScene;
extern std::shared_ptr<Scene> gameScene;

constexpr float gameWidth = 800.0f;
constexpr float gameHeight = 600.0f;


// Forward declaration of MenuScene class
class MenuScene final : public Scene {
private:
    sf::Font font;
    sf::Text text;

public:
    MenuScene() = default;
    void update(double dt) override;
    void render() override;
    void load()override;
};

// Forward declaration of GameScene class
class GameScene final : public Scene {
private:
    sf::Text text;
    sf::Clock scoreClock;
    void respawn();

public:
    GameScene() = default;
    void update(double dt) override;
    void render() override;
    void load() override;
};