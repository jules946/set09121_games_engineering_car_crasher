//car_crasher.h
#pragma once
#include "scene.h"
#include "obstacle_manager.h"
#include "background_manager.h"
#include "collision_manager.h"
#include "game_UI_Manager.h"
#include <SFML/Graphics.hpp>
#include <memory>

// Forward declaration of MenuScene class
class MenuScene final : public Scene {
private:
    Font font;
    Text text;
    Text promptText;
    Clock blinkClock;
    bool showPrompt;

public:
    MenuScene() : showPrompt(true) {}
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
    std::shared_ptr<Entity> _cop;
    CollisionManager _collisionManager;
    gameUIManager _gameUIManager;

    Font font;
    Text livesText;
    Text scoreText;
    Text pauseText;
    bool _firstUpdate = true;
public:
    GameScene() : _obstacleManager(nullptr) {}
    void update(double dt) override;
    void reset() override;

    void render() override;
    void load() override;
};


// Forward declaration of PauseScene class
class PauseScene final : public Scene {
protected:
    Font font;
    std::vector<std::shared_ptr<Entity>> _menuItems;

public:
    void load() override;
    void update(double dt) override;
    void render() override;
};

// Forward declaration of GameOverScene class
class GameOverScene final : public Scene {
private:
    Font font;
    Text gameOverText;
    Text gameOverScoreText;
    Text promptText;
    gameUIManager _gameUIManager;
public:
    void load() override;
    void update(double dt) override;
    void render() override;
};


// Forward declaration of KeyBindScene class
class KeyBindScene final : public Scene {
private:
    Font font;
    Text titleText;
    Text leftKeyText;
    Text rightKeyText;
    Text promptText;

public:
    void load() override;
    void update(double dt) override;
    void render() override;
};


// Forward declaration of ChangeCarScene class
class ChangeCarScene final : public Scene {
private:
    Font font;
    Text titleText;
    std::vector<std::pair<Text, std::string>> carOptions;
    size_t selectedOption = 0;

public:
    void load() override;
    void update(double dt) override;
    void render() override;
};
