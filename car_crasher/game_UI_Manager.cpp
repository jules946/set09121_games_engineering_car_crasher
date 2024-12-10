//game_UI_Manager.cpp
#include "cmp_sprite.h"
#include "car_crasher.h"
#include "game_config.h"
#include "game_UI_Manager.h"
#include <SFML/Graphics.hpp>

using namespace sf;

int livesInt = 3;

void gameUIManager::loadLives(EntityManager& entityManager, int livesInt) {
  // TODO add heart image
    if (!livesTexture.loadFromFile("res/img/heartLife.png")) {
        throw std::runtime_error("Failed to load road texture!");
    }

    //const sf::Vector2f textureSize(livesTexture.getSize().x, livesTexture.getSize().y);
    //const float scaleX = columnWidth / textureSize.x;
    //const float scaledWidth = textureSize.x * scaleX;

    for (int i = 0; i < livesInt; i++) {
        auto heart = std::make_shared<Entity>();

        float yPos = 20.f;
        float xPos = gameWidth - 150.f - i*45;

        heart->setPosition(sf::Vector2f(xPos, yPos));

        auto s = heart->addComponent<SpriteComponent>();
        s->getSprite().setTexture(livesTexture);
        s->getSprite().setScale(0.1f, 0.1f);
        s->getSprite().setOrigin(0, 0);
        //s->getSprite().setPosition(sf::Vector2f(xPos, yPos));
        
        entityManager.list.push_back(heart);
    }
}


void gameUIManager::update(double dt, EntityManager& entityManager) {
    auto& entities = entityManager.list;
    std::vector<std::shared_ptr<Entity>> uiHearts;

    // Increment score
    score++;

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
        float yPos = 20.f;
        float xPos = gameWidth - 150.f - uiHearts.size() * 45;
        heart->setPosition(sf::Vector2f(xPos, yPos));

        auto s = heart->addComponent<SpriteComponent>();
        s->getSprite().setTexture(livesTexture);
        s->getSprite().setScale(0.1f, 0.1f);
        s->getSprite().setOrigin(0, 0);

        entityManager.list.push_back(heart);
        uiHearts.push_back(heart);
    }
}

void gameUIManager::loadLivesText(Font& font, Text& livesText) {
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
}

void gameUIManager::loadScoreText(Font& font, Text& scoreText) {
    int tempScore = score / 60;
    std::string s = std::to_string(tempScore);

    scoreText.setFont(font); // Set the font of the text
    scoreText.setCharacterSize(24); // Set the character size
    scoreText.setFillColor(sf::Color::White); // Set the text color
    scoreText.setString("Score:" + s);
    // Get text bounds
    const FloatRect bounds = scoreText.getLocalBounds();

    // Ensuring the text's origin and position are aligned to whole pixels to avoid subpixel
    // rendering and prevent blurriness
    scoreText.setOrigin(std::round(bounds.width / 2.f), std::round(bounds.height / 2.f));
    scoreText.setPosition(gameWidth - 240.f, 100.f);
}

void gameUIManager::loadPauseText(Font& font, Text& pauseText) {
    pauseText.setFont(font);
    pauseText.setCharacterSize(24);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setString("Press Tab to pause");

    const FloatRect bounds = pauseText.getLocalBounds();
    pauseText.setOrigin(std::round(bounds.width / 2.f), std::round(bounds.height / 2.f));
    pauseText.setPosition(80.f, 40.f);
}

void gameUIManager::loadGameOverText(Font& font, Text& gameOverText, Text& gameOverScoreText, Text& promptText) {
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(96);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over");
    // Get text bounds
    const FloatRect bounds = gameOverText.getLocalBounds();

    // Ensuring the text's origin and position are aligned to whole pixels to avoid subpixel
    // rendering and prevent blurriness
    gameOverText.setOrigin(std::round(bounds.width / 2.f), std::round(bounds.height / 2.f));
    gameOverText.setPosition(gameWidth / 2.f, gameHeight / 2.f - 100);

    int tempScore = score / 60;
    std::string s = std::to_string(tempScore);

    gameOverScoreText.setFont(font); // Set the font of the text
    gameOverScoreText.setCharacterSize(72); // Set the character size
    gameOverScoreText.setFillColor(sf::Color::Red); // Set the text color
    gameOverScoreText.setString("Score:" + s);
    // Get text bounds
    const FloatRect bounds2 = gameOverScoreText.getLocalBounds();

    // Ensuring the text's origin and position are aligned to whole pixels to avoid subpixel
    // rendering and prevent blurriness
    gameOverScoreText.setOrigin(std::round(bounds2.width / 2.f), std::round(bounds2.height / 2.f));
    gameOverScoreText.setPosition(gameWidth / 2.f, gameHeight / 2.f + 100);

    promptText.setFont(font);
    promptText.setString("Press Enter to go to the Main Menu");
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(gameWidth / 2.f, gameHeight / 2.f);
    promptText.setOrigin(promptText.getLocalBounds().width / 2.f,
        promptText.getLocalBounds().height / 2.f);
}

