// cmp_menu.cpp
#include "cmp_menu.h"

#include "car_crasher.h"
#include "game_config.h"
#include "system_renderer.h"
#include "scene.h"


// TextComponent implementation
TextComponent::TextComponent(Entity* p, const std::string& str)
    : Component(p), _selected(false) {
    _font = std::make_shared<sf::Font>();
    if (!_font->loadFromFile("res/fonts/PixelifySans-VariableFont_wght.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }
    _text.setFont(*_font);
    _text.setString(str);
}


void TextComponent::update(double dt) {
    _text.setPosition(_parent->getPosition());
    if (_selected) {
        _text.setFillColor(sf::Color::Yellow);
    } else {
        _text.setFillColor(sf::Color::White);
    }
}

void TextComponent::render() {
    Renderer::queue(&_text);
}

void TextComponent::setText(const std::string& str) {
    _text.setString(str);
}

void TextComponent::setSelected(bool selected) {
    _selected = selected;
}

void TextComponent::setCharacterSize(unsigned int size) {
    _text.setCharacterSize(size);
}

void TextComponent::centerOrigin() {
    const auto bounds = _text.getLocalBounds();
    _text.setOrigin(std::round(bounds.width / 2.f),
                    std::round(bounds.height / 2.f));
}


// MenuComponent implementation
MenuComponent::MenuComponent(Entity* p, const MenuType type)
    : Component(p),
      _state(MenuState::TITLE),
      _type(type),
      _selectedOption(0) { }

void MenuComponent::update(double dt) {
    static bool upPressed = false;
    static bool downPressed = false;
    static bool spacePressed = false;
    static bool returnPressed = false;
    static sf::Clock inputDelay;

    if (_type == MenuType::MAIN) {
        // Main Menu Logic
        if (_state == MenuState::TITLE) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !returnPressed) {
                _state = MenuState::MAIN_MENU;
                returnPressed = true;
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                returnPressed = false;
            }
        }
    }
    else if (_type == MenuType::PAUSE) {
        // Pause Menu Logic - Always in PAUSE_MENU state
        _state = MenuState::PAUSE_MENU;
    }

    // Common navigation code for both menu types
    if (_state != MenuState::TITLE) {
        if (inputDelay.getElapsedTime().asSeconds() > 0.15f) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed) {
                _menuItems[_selectedOption]->getComponent<TextComponent>()->setSelected(false);
                _selectedOption = (_selectedOption - 1 + _menuItems.size()) % _menuItems.size();
                _menuItems[_selectedOption]->getComponent<TextComponent>()->setSelected(true);
                upPressed = true;
                inputDelay.restart();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed) {
                _menuItems[_selectedOption]->getComponent<TextComponent>()->setSelected(false);
                _selectedOption = (_selectedOption + 1) % _menuItems.size();
                _menuItems[_selectedOption]->getComponent<TextComponent>()->setSelected(true);
                downPressed = true;
                inputDelay.restart();
            }
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) upPressed = false;
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) downPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !returnPressed) {
            if (_type == MenuType::MAIN) {
                if (_selectedOption == 0) {  // Play Game
                    activeScene = gameScene;
                } else if (_selectedOption == 1) {
                    activeScene = changeCarScene;
                    sf::sleep(sf::milliseconds(100));  // Add a small delay to ensure no carry-over
                }
                else if (_selectedOption == 3) {  // Key Binds option
                    activeScene = keyBindScene;
                    sf::sleep(sf::milliseconds(100));  // Add a small delay to ensure no carry-over
                }
            } else if (_type == MenuType::PAUSE) {
                if (_selectedOption == 0) {  // "Yes, please"
                    activeScene = menuScene;
                    gameScene->reset();
                } else if (_selectedOption == 1) {  // "No, go back to the game"
                    activeScene = gameScene;
                    gameScene->resumeSounds();


                }
            }
            returnPressed = true;  // Set returnPressed only after scene transition logic
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            returnPressed = false;  // Reset when Return key is released
        }

    }

    // Update visibility
    for (auto& item : _menuItems) {
        item->setVisible(_state != MenuState::TITLE);
    }
}


void MenuComponent::addMenuItem(const std::shared_ptr<Entity>& item) {
    _menuItems.push_back(item);
}

