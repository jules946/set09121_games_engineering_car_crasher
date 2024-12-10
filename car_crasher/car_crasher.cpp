//car_crasher.cpp
#include <cmath>
#include <memory>

#include "car_crasher.h"
#include "cmp_player_movement.h"
#include "game_config.h"
#include "cmp_sound_effect.h"
#include "ecm.h"
#include "cmp_sprite.h"
#include "system_renderer.h"
#include "background_manager.h"
#include "cmp_hit_box.h"
#include "obstacle_manager.h"
#include "game_UI_Manager.h"
#include "cmp_menu.h"
#include "cmp_police_ai_movement.h"


// TODO
// Obstacle Manager -- Alessio
//- add good obstacles e.g. heart
//- if car drives over heart ++lives

//Main Menu -- Alessio
//- Add text to let player know to click a button to start game - Done
//- Add car change functionality
//- Add keybind functionality
//- Add difficulity functionality
//- Add pause menu to gameScene - Done


using namespace sf;
using namespace std;


// MenuScene class implementation
void MenuScene::load() {
    if (!font.loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    // Debug before creating component
    std::cout << "Before component creation - gameScene: " << gameScene << std::endl;

    // Create menu controller entity
    auto menuEntity = std::make_shared<Entity>();
    auto menuControl = menuEntity->addComponent<MenuComponent>(activeScene, gameScene, MenuComponent::MenuType::MAIN);
    // Debug after
    std::cout << "After component creation - gameScene: " << gameScene << std::endl;

    // Create title
    auto title = std::make_shared<Entity>();
    auto titleText = title->addComponent<TextComponent>("Car Crasher");
    titleText->setCharacterSize(48);
    title->setPosition(Vector2f(gameWidth / 2.f, gameHeight / 3.f));
    titleText->centerOrigin();


    // Create menu items
    std::vector<std::string> options = {
        "Play Game",
        "Choose Different Car",
        "Difficulty",
        "Key Binds"
    };

    _entity_manager.list.push_back(menuEntity);
    _entity_manager.list.push_back(title);

    float yPos = gameHeight / 2.f;
    for (const auto& opt : options) {
        auto item = std::make_shared<Entity>();
        auto text = item->addComponent<TextComponent>(opt);
        text->setCharacterSize(32);
        item->setPosition(Vector2f(gameWidth / 2.f, yPos));
        text->centerOrigin();
        menuControl->addMenuItem(item);
        _entity_manager.list.push_back(item);
        yPos += 60.f;
    }

    // Setup prompt text
    promptText.setFont(font);
    promptText.setString("Press Enter to continue or to select an item in the Menu. Up and Down to navigate the Menu");
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);

    // Center the prompt text
    const FloatRect promptBounds = promptText.getLocalBounds();
    promptText.setOrigin(std::round(promptBounds.width / 2.f),
                        std::round(promptBounds.height / 2.f));
    promptText.setPosition(std::round(gameWidth / 2.f),
                          std::round(gameHeight - 100.f));
}


void MenuScene::update(const double dt) {
    // Blink the prompt text every 0.8 seconds
    if (blinkClock.getElapsedTime().asSeconds() > 0.8f) {
        showPrompt = !showPrompt;
        blinkClock.restart();
    }

    Scene::update(dt);
}


void MenuScene::render() {
    Scene::render();
    if (showPrompt) {
        Renderer::queue(&promptText);
    }
}

// GameScene class implementation
void GameScene::load() {
    // add background
    _backgroundManager.loadBackground(_entity_manager);

    // Initialize obstacle manager
    _obstacleManager = std::make_unique<ObstacleManager>(_entity_manager);
    _obstacleManager->initializeSprites(); // Single function call to add all sprites

    // Create player
    _player = make_shared<Entity>();

    // Add sprite component to player
    const auto s = _player->addComponent<SpriteComponent>();
    s->setTexture("res/img/BlueCar.png");
    s->getSprite().setScale(2.0f, 2.0f);
    s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2.f,
                             s->getSprite().getLocalBounds().height / 2.f);

    // Set initial position of player to second lane
    _player->setPosition(Vector2f(lanePositions[1], gameHeight / 2.f));

    // Add components to player
    _player->addComponent<SoundEffectComponent>("res/sound/tires_squal_loop.wav");
    _player->addComponent<PlayerMovementComponent>(lanePositions);
    _player->addComponent<HitboxComponent>(FloatRect(0, 0, s->getSprite().getLocalBounds().width, s->getSprite().getLocalBounds().height));

    _entity_manager.list.push_back(_player);

    // TODO maybe make part of the gameScene class?
    // Create cop car
    _cop = make_shared<Entity>();

    const auto copSprite = _cop->addComponent<SpriteComponent>();
    copSprite->setTexture("res/img/police_car.png");
    copSprite->getSprite().setScale(2.0f, 2.0f);
    copSprite->getSprite().setOrigin(copSprite->getSprite().getLocalBounds().width / 2.f,
                             copSprite->getSprite().getLocalBounds().height / 2.f);

    _cop->setPosition(Vector2f(lanePositions[2], gameHeight + 100.f)); // Start below screen

    // Add hitbox component
    _cop->addComponent<HitboxComponent>(FloatRect(0, 0,
        copSprite->getSprite().getLocalBounds().width,
        copSprite->getSprite().getLocalBounds().height));

    // Add pursuit component with required parameters
    _cop->addComponent<PolicePursuitComponent>(lanePositions, _player, &_entity_manager);
    _cop->addComponent<SoundEffectComponent>("res/sound/police_siren.wav");

    _entity_manager.list.push_back(_cop);


    // UI for lives etc
    if (!font.loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }
    livesText.setFont(font); // Set the font of the text
    livesText.setCharacterSize(24); // Set the character size
    livesText.setFillColor(sf::Color::White); // Set the text color
    livesText.setString("Lives:");
    // Get text bounds
    const FloatRect bounds = livesText.getLocalBounds();

    // Ensuring the text's origin and position are aligned to whole pixels to avoid subpixel
    // rendering and prevent blurriness
    livesText.setOrigin(std::round(bounds.width / 2.f), std::round(bounds.height / 2.f));
    livesText.setPosition(gameWidth - 270.f, 40.f);
    _gameUIManager.loadLives(_entity_manager, livesInt);
}

void GameScene::update(const double dt) {
    if (_firstUpdate) {
        const auto siren = _cop->getComponent<SoundEffectComponent>();
        if (siren) {
            siren->playSound();
        }
        _firstUpdate = false;
    }
    // std::cout << "Game scene update" << std::endl;
    if (Keyboard::isKeyPressed(Keyboard::Tab)) {
        activeScene = menuScene;
    }

    // Update collision manager
    CollisionManager::checkPlayerCollisions(_entity_manager, _player, _cop);

    // Update obstacle manager
    if (_obstacleManager) {
        _obstacleManager->update(dt);
    }
    
    _gameUIManager.update(dt, _entity_manager, livesInt);

    if (livesInt <= 0 && activeScene != gameOverScene) {
        pauseSounds();
        activeScene = gameOverScene;
    }

    Scene::update(dt);
}

// Add all entities to the renderer queue
void GameScene::render() {
    Renderer::queue(&livesText);
    Scene::render();
}


// Created to stop the screeching sound while the game is paused
void GameScene::pauseSounds() {
    if (auto sound = _player->getComponent<SoundEffectComponent>()) {
        sound->stopSound();
    }

    for (auto& entity : _entity_manager.list) {
        if (auto sound = entity->getComponent<SoundEffectComponent>()) {
            sound->stopSound();
        }
    }
}

void PauseScene::load() {
    if (!font.loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    // Create pause title
    auto title = std::make_shared<Entity>();
    auto titleText = title->addComponent<TextComponent>("Game Paused");
    titleText->setCharacterSize(48);
    title->setPosition(Vector2f(gameWidth / 2.f, gameHeight / 3.f));
    titleText->centerOrigin();

    // Create question text
    auto question = std::make_shared<Entity>();
    auto questionText = question->addComponent<TextComponent>("Do you want to quit this game session?");
    questionText->setCharacterSize(32);
    question->setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));
    questionText->centerOrigin();
    // Create menu options
    std::vector<std::string> options = {
        "Yes, please",
        "No, go back to the game"
    };

    float yPos = gameHeight / 1.5f;
    auto menuEntity = std::make_shared<Entity>();
    auto menuControl = menuEntity->addComponent<MenuComponent>(activeScene, gameScene, MenuComponent::MenuType::PAUSE);

    for (const auto& opt : options) {
        auto item = std::make_shared<Entity>();
        auto text = item->addComponent<TextComponent>(opt);
        text->setCharacterSize(32);
        item->setPosition(Vector2f(gameWidth / 2.f, yPos));
        text->centerOrigin();
        menuControl->addMenuItem(item);
        _entity_manager.list.push_back(item);
        yPos += 60.f;
    }

    _entity_manager.list.push_back(title);
    _entity_manager.list.push_back(question);
    _entity_manager.list.push_back(menuEntity);
}

void PauseScene::update(double dt) {
    Scene::update(dt);
}

void PauseScene::render() {
    Scene::render();
}


void GameOverScene::load() {
    if (!font.loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(gameWidth / 2.f, gameHeight / 3.f);
    gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2.f,
                          gameOverText.getLocalBounds().height / 2.f);

    promptText.setFont(font);
    promptText.setString("Press Enter to go to the Main Menu");
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(gameWidth / 2.f, gameHeight / 2.f);
    promptText.setOrigin(promptText.getLocalBounds().width / 2.f,
                        promptText.getLocalBounds().height / 2.f);
}

void GameOverScene::update(double dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
        livesInt = 3;  // Reset lives
        gameScene = std::make_shared<GameScene>();
        gameScene->load();
        activeScene = menuScene;
    }
    Scene::update(dt);
}

void GameOverScene::render() {
    Renderer::queue(&gameOverText);
    Renderer::queue(&promptText);
    Scene::render();
}