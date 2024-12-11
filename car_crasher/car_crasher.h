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
    sf::Font font;
    sf::Text text;
    sf::Text promptText;
    sf::Clock blinkClock;
    bool showPrompt;

public:
    MenuScene() : showPrompt(true) {}
    // MenuScene() = default;
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

    sf::Font font;
    sf::Text livesText;
    sf::Text scoreText;
    sf::Text pauseText;
    bool _firstUpdate = true;
public:
    GameScene() : _obstacleManager(nullptr) {}
    void update(double dt) override;
    void updateCarSprite() const;
    void reset() override;

    void render() override;
    void load() override;
};

class PauseScene : public Scene {
protected:
    sf::Font font;
    std::vector<std::shared_ptr<Entity>> _menuItems;

public:
    void load() override;
    void update(double dt) override;
    void render() override;
};

class GameOverScene : public Scene {
private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text gameOverScoreText;
    sf::Text promptText;
    gameUIManager _gameUIManager;
public:
    void load() override;
    void update(double dt) override;
    void render() override;
};

class KeyBindScene : public Scene {
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text leftKeyText;
    sf::Text rightKeyText;
    sf::Text promptText;

public:
    void load() override;
    void update(double dt) override;
    void render() override;
};

class ChangeCarScene : public Scene {
private:
    sf::Font font;
    sf::Text titleText;
    std::vector<std::pair<sf::Text, std::string>> carOptions;
    size_t selectedOption = 0;

public:
    void load() override;
    void update(double dt) override;
    void render() override;
};
