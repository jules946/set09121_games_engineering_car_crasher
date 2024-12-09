//car_crasher.h
#pragma once
#include "scene.h"
#include "obstacle_manager.h"
#include "background_manager.h"
#include "collision_manager.h"
#include "game_UI_Manager.h"
#include <SFML/Graphics.hpp>
#include <memory>

/* Now in scene.cpp
extern std::shared_ptr<Scene> activeScene;
extern std::shared_ptr<Scene> menuScene;
extern std::shared_ptr<Scene> gameScene;
*/
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
    std::unique_ptr<ObstacleManager> _obstacleManager;
    backgroundManager _backgroundManager;
    std::shared_ptr<Entity> _player;
    CollisionManager _collisionManager;
    gameUIManager _gameUIManager;
    sf::Font font;
    sf::Text livesText;
public:
    GameScene() : _obstacleManager(nullptr) {}
    void update(double dt) override;
    void render() override;
    void load() override;
};
