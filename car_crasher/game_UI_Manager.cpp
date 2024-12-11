//game_UI_Manager.cpp
#include "cmp_sprite.h"
#include "car_crasher.h"
#include "game_config.h"
#include "game_UI_Manager.h"
#include <SFML/Graphics.hpp>

using namespace sf;

int livesInt = 3;

// Load the heart texture and create the hearts based on the livesInt
void gameUIManager::loadLives(EntityManager& entityManager, const int livesInt) {
    if (!livesTexture.loadFromFile("res/img/heartLife.png")) {
        throw std::runtime_error("Failed to load road texture!");
    }

    for (int i = 0; i < livesInt; i++) {
        auto heart = std::make_shared<Entity>();

        constexpr float yPos = 20.f;
        const float xPos = gameWidth - 150.f - i*45;

        heart->setPosition(Vector2f(xPos, yPos));

        const auto s = heart->addComponent<SpriteComponent>();
        s->getSprite().setTexture(livesTexture);
        s->getSprite().setScale(0.1f, 0.1f);
        s->getSprite().setOrigin(0, 0);
        
        entityManager.list.push_back(heart);
    }
}

// Initial setup of all text properties
void gameUIManager::setupTexts(Text& scoreText, Text& livesText, Text& pauseText) const {
    // Score text setup
    scoreText.setFont(_font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(gameWidth - 240.f, 100.f);

    // Lives text setup
    livesText.setFont(_font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(gameWidth - 270.f, 40.f);
    livesText.setString("Lives:");
    const FloatRect livesBounds = livesText.getLocalBounds();
    livesText.setOrigin(std::round(livesBounds.width / 2.f),
                       std::round(livesBounds.height / 2.f));

    // Pause text setup
    pauseText.setFont(_font);
    pauseText.setCharacterSize(24);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setString("Press Tab to pause");
    const FloatRect pauseBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(std::round(pauseBounds.width / 2.f),
                       std::round(pauseBounds.height / 2.f));
    pauseText.setPosition(std::round(150.f), std::round(40.f));

    // Initial score update
    updateScoreText(scoreText);
}

// Just update the score text content
void gameUIManager::updateScoreText(Text& scoreText) {
    scoreText.setString("Score: " + std::to_string(score));
    const FloatRect bounds = scoreText.getLocalBounds();
    scoreText.setOrigin(std::round(bounds.width / 2.f),
                       std::round(bounds.height / 2.f));
}

// Update the UI elements
void gameUIManager::update(double dt, EntityManager& entityManager, Text& scoreText) const {
    auto& entities = entityManager.list;
    std::vector<std::shared_ptr<Entity>> uiHearts;

    // Add points based on time elapsed
    static float timeSinceLastScore = 0.0f;
    timeSinceLastScore += dt;

    if (timeSinceLastScore >= 0.5f) {
        score += 10;
        timeSinceLastScore = 0.0f;
    }
    updateScoreText(scoreText);
    // Collect UI hearts
    for (const auto& entity : entities) {
        if (entity->getPosition().y == 20.f) {
            uiHearts.push_back(entity);
        }
    }

    // Remove excess hearts
    while (uiHearts.size() > livesInt) {
        uiHearts.back()->setForDelete();
        uiHearts.pop_back();
    }

    // Add missing hearts
    while (uiHearts.size() < livesInt) {
        auto heart = std::make_shared<Entity>();
        constexpr float yPos = 20.f;
        const float xPos = gameWidth - 150.f - uiHearts.size() * 45;
        heart->setPosition(Vector2f(xPos, yPos));

        const auto s = heart->addComponent<SpriteComponent>();
        s->getSprite().setTexture(livesTexture);
        s->getSprite().setScale(0.1f, 0.1f);
        s->getSprite().setOrigin(0, 0);

        entityManager.list.push_back(heart);
        uiHearts.push_back(heart);
    }
}

void gameUIManager::setupGameOverTexts(Text& gameOverText, Text& gameOverScoreText, Text& promptText) const {
    // Game Over text setup
    gameOverText.setFont(_font);
    gameOverText.setCharacterSize(96);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over");
    const FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(std::round(bounds.width / 2.f), std::round(bounds.height / 2.f));
    gameOverText.setPosition(gameWidth / 2.f, gameHeight / 2.f - 100);

    // Score text setup
    gameOverScoreText.setFont(_font);
    gameOverScoreText.setCharacterSize(72);
    gameOverScoreText.setFillColor(sf::Color::Red);

    // Prompt text setup
    promptText.setFont(_font);
    promptText.setString("Press Enter to go to the Main Menu");
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(gameWidth / 2.f, gameHeight / 2.f);
    promptText.setOrigin(promptText.getLocalBounds().width / 2.f,
        promptText.getLocalBounds().height / 2.f);

    // Initial score update
    updateGameOverScore(gameOverScoreText);
}

// Just update the score text
void gameUIManager::updateGameOverScore(Text& gameOverScoreText) {
    const std::string s = std::to_string(score);
    gameOverScoreText.setString("Score: " + s);
    const FloatRect bounds = gameOverScoreText.getLocalBounds();
    gameOverScoreText.setOrigin(std::round(bounds.width / 2.f), std::round(bounds.height / 2.f));
    gameOverScoreText.setPosition(gameWidth / 2.f, gameHeight / 2.f + 100);
}

void gameUIManager::loadFont(const Font& font) {
    _font = font;
}
