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
#include "cmp_key_binds.h"
#include "obstacle_manager.h"
#include "game_UI_Manager.h"
#include "cmp_menu.h"
#include "cmp_police_ai_movement.h"


using namespace sf;
using namespace std;

int score;
bool isHardDifficulty = false;

std::unique_ptr<KeyBindComponent> keyBindComponent;

// Default car selection to Blue Car
std::string selectedCar = "Blue Car";

// Map to hold different car res paths
const std::map<std::string, std::string> CAR_CONFIGS = {
    std::make_pair("Blue Car", "res/img/BlueCar.png"),
    std::make_pair("Grey Car", "res/img/Car_Grey.png"),
    std::make_pair("Red Car", "res/img/Car_Red.png"),
    std::make_pair("Striped Car", "res/img/Car_Striped.png")
};

void MenuScene::load() {
    if (!font.loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    // Create menu controller entity
    const auto menuEntity = std::make_shared<Entity>();
    const auto menuControl = menuEntity->addComponent<MenuComponent>(MenuComponent::MenuType::MAIN);

    // Create title
    const auto title = std::make_shared<Entity>();
    const auto titleText = title->addComponent<TextComponent>("Car Crasher");
    titleText->setCharacterSize(48);
    title->setPosition(Vector2f(gameWidth / 2.f, gameHeight / 3.f));
    titleText->centerOrigin();


    // Create menu items
    std::vector<std::string> options = {
        "Play Game",
        "Choose Different Car",
        "Difficulty: " + std::string(isHardDifficulty ? "Hard" : "Easy"),
        "Key Binds",
        "Quit Game"
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
    score = 0;
    // add background
    _backgroundManager.loadBackground(_entity_manager);

    // Initialize obstacle manager
    _obstacleManager = std::make_unique<ObstacleManager>(_entity_manager);
    _obstacleManager->initializeSprites();

    // Create player
    _player = make_shared<Entity>();

    // std::cout << "Loading car: " << static_cast<int>(selectedCar) << std::endl;
    const auto s = _player->addComponent<SpriteComponent>(); // using const
    s->setTexture(CAR_CONFIGS.at(selectedCar));
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

    _gameUIManager.loadFont(font);
    _gameUIManager.setupTexts(scoreText, livesText, pauseText);
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
    if (Keyboard::isKeyPressed(Keyboard::Tab)) {
        stopSounds();
        activeScene = pauseScene;
    }
    
    // Update collision manager
    CollisionManager::checkPlayerCollisions(_entity_manager, _player, _cop);

    // Update obstacle manager
    if (_obstacleManager) {
        _obstacleManager->update(dt);
    }
  

    _gameUIManager.update(dt, _entity_manager, scoreText);

    if (livesInt <= 0 && activeScene != gameOverScene) {
        stopSounds();
        std::cout << "Debug: Score before switching to game over scene: " << score << std::endl;
        activeScene = gameOverScene;
    }

    Scene::update(dt);
}

void GameScene::reset() {
    livesInt = 3;
    score = 0;
    _firstUpdate = true;
    _entity_manager.list.clear();
    load();
}

// Add all entities to the renderer queue
void GameScene::render() {
    Scene::render();
    Renderer::queue(&livesText);
    Renderer::queue(&scoreText);
    Renderer::queue(&pauseText);
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
    auto menuControl = menuEntity->addComponent<MenuComponent>(MenuComponent::MenuType::PAUSE);

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
    _gameUIManager.loadFont(font);
    _gameUIManager.setupGameOverTexts(gameOverText, gameOverScoreText, promptText);
}

void GameOverScene::update(double dt) {
    _gameUIManager.updateGameOverScore(gameOverScoreText);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
        gameScene->reset();
        activeScene = menuScene;
    }
    Scene::update(dt);
}

void GameOverScene::render() {
    Renderer::queue(&gameOverText);
    Renderer::queue(&gameOverScoreText);
    Renderer::queue(&promptText);
    Scene::render();
}

void KeyBindScene::load() {
    // Create the KeyBindComponent instance
    auto entity = makeEntity();
    keyBindComponent = std::make_unique<KeyBindComponent>(entity.get());

    // Load fonts and setup is already handled in KeyBindComponent, so no need to duplicate here
    keyBindComponent->update(0); // Ensure initial state is set
}

void KeyBindScene::update(double dt) {
    // Update the KeyBindComponent
    keyBindComponent->update(dt);

    // Handle scene navigation (return to menu)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
        activeScene = menuScene;
    }

    Scene::update(dt);
}

void KeyBindScene::render() {
    // Render the KeyBindComponent
    keyBindComponent->render();

    Scene::render();
}

void ChangeCarScene::load() {
    if (!font.loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    titleText.setFont(font);
    titleText.setString("Choose Your Car");
    titleText.setCharacterSize(48);
    titleText.setOrigin(std::round(titleText.getLocalBounds().width / 2.f), std::round(titleText.getLocalBounds().height / 2.f));
    titleText.setPosition(std::round(gameWidth / 2.f), std::round(gameHeight / 4.f));

    carOptions.clear();
    float yPos = gameHeight / 2.f;

    // Create menu items from CAR_CONFIGS
    for (const auto& carConfig : CAR_CONFIGS) {
        const std::string& carName = carConfig.first;
        const std::string& spritePath = carConfig.second;

        sf::Text text;
        text.setFont(font);
        text.setString(carName);
        text.setCharacterSize(32);
        text.setOrigin(std::round(text.getLocalBounds().width / 2.f), std::round(text.getLocalBounds().height / 2.f));
        text.setPosition(std::round(gameWidth / 2.f), std::round(yPos));
        carOptions.emplace_back(text, spritePath);
        yPos += 60.f;
    }

    // Set initial selected option based on current selectedCar
    selectedOption = 0;
    int index = 0;
    for (const auto& carConfig : CAR_CONFIGS) {
        const std::string& carName = carConfig.first;
        if (carName == selectedCar) {
            selectedOption = index;
            break;
        }
        index++;
    }
}

void ChangeCarScene::update(double dt) {
    static bool upPressed = false;
    static bool downPressed = false;
    static bool returnPressed = false;
    static sf::Clock inputDelay;

    if (inputDelay.getElapsedTime().asSeconds() > 0.15f) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed) {
            selectedOption = (selectedOption - 1 + carOptions.size()) % carOptions.size();
            upPressed = true;
            inputDelay.restart();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed) {
            selectedOption = (selectedOption + 1) % carOptions.size();
            downPressed = true;
            inputDelay.restart();
        }
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) upPressed = false;
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) downPressed = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !returnPressed) {
        // Convert selectedOption index to car name using the options vector
        auto it = CAR_CONFIGS.begin();
        std::advance(it, selectedOption);
        selectedCar = it->first;  // Set the selected car name

        std::cout << "Selected car: " << selectedCar << std::endl;
        activeScene = menuScene;
        returnPressed = true;
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) returnPressed = false;

    Scene::update(dt);
}


void ChangeCarScene::render() {
    Renderer::queue(&titleText);
    for (size_t i = 0; i < carOptions.size(); ++i) {
        carOptions[i].first.setFillColor(i == selectedOption ? sf::Color::Yellow : sf::Color::White);
        Renderer::queue(&carOptions[i].first);
    }
}