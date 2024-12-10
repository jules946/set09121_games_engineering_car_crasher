// cmp_key_binds.cpp
#include "cmp_key_binds.h"
#include "game_config.h"
#include "system_renderer.h"
#include "scene.h"
#include <map>
#include <SFML/Window/Keyboard.hpp>

sf::Keyboard::Key KeyBindComponent::_leftKey = sf::Keyboard::Left;
sf::Keyboard::Key KeyBindComponent::_rightKey = sf::Keyboard::Right;

KeyBindComponent::KeyBindComponent(Entity* p) : Component(p) {
    if (!_font.loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    _titleText.setFont(_font);
    _titleText.setString("Key Binds");
    _titleText.setCharacterSize(48);
    _titleText.setPosition(gameWidth / 2.f, gameHeight / 4.f);
    _titleText.setOrigin(_titleText.getLocalBounds().width / 2.f,
                       _titleText.getLocalBounds().height / 2.f);

    _leftKeyText.setFont(_font);
    _leftKeyText.setString("Left Key: A");
    _leftKeyText.setCharacterSize(32);
    _leftKeyText.setPosition(gameWidth / 2.f, gameHeight / 2.f - 50.f);
    _leftKeyText.setOrigin(_leftKeyText.getLocalBounds().width / 2.f,
                         _leftKeyText.getLocalBounds().height / 2.f);

    _rightKeyText.setFont(_font);
    _rightKeyText.setString("Right Key: D");
    _rightKeyText.setCharacterSize(32);
    _rightKeyText.setPosition(gameWidth / 2.f, gameHeight / 2.f + 50.f);
    _rightKeyText.setOrigin(_rightKeyText.getLocalBounds().width / 2.f,
                          _rightKeyText.getLocalBounds().height / 2.f);

    _promptText.setFont(_font);
    _promptText.setString("Click on key to change, ESC to return");
    _promptText.setCharacterSize(24);
    _promptText.setPosition(gameWidth / 2.f, gameHeight * 3.f / 4.f);
    _promptText.setOrigin(_promptText.getLocalBounds().width / 2.f,
                        _promptText.getLocalBounds().height / 2.f);
}

void KeyBindComponent::update(double dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        activeScene = menuScene;
        return;
    }

    if (_waitingForLeft) {
        for (int k = 0; k < sf::Keyboard::KeyCount; ++k) {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k))) {
                _leftKey = static_cast<sf::Keyboard::Key>(k);
                _waitingForLeft = false;
                _leftKeyText.setString("Left Key: " + std::to_string(k));
                break;
            }
        }
    }
    else if (_waitingForRight) {
        for (int k = 0; k < sf::Keyboard::KeyCount; ++k) {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k))) {
                _rightKey = static_cast<sf::Keyboard::Key>(k);
                _waitingForRight = false;
                _rightKeyText.setString("Right Key: " + std::to_string(k));
                break;
            }
        }
    }
    else {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition();
            if (_leftKeyText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                _waitingForLeft = true;
                _leftKeyText.setString("Press new key for Left...");
            }
            else if (_rightKeyText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                _waitingForRight = true;
                _rightKeyText.setString("Press new key for Right...");
            }
        }
    }
}

void KeyBindComponent::render() {
    Renderer::queue(&_titleText);
    Renderer::queue(&_leftKeyText);
    Renderer::queue(&_rightKeyText);
    Renderer::queue(&_promptText);
}