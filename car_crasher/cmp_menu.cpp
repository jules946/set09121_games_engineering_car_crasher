// cmp_menu.cpp
#include "cmp_menu.h"
#include "system_renderer.h"
#include "game_config.h"
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
MenuComponent::MenuComponent(Entity* p,
                           std::shared_ptr<Scene>& activeScene,
                           std::shared_ptr<Scene>& gameScene)
    : Component(p),
      _state(MenuState::TITLE),
      _selectedOption(0),
      _activeScene(activeScene),
      _gameScene(gameScene) {    std::cout << "MenuComponent Constructor - gameScene: " << _gameScene << std::endl;}

void MenuComponent::update(double dt) {
    static bool upPressed = false;
    static bool downPressed = false;
    static bool spacePressed = false;
    static bool returnPressed = false;
    static sf::Clock inputDelay;

    if (_state == MenuState::TITLE) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacePressed) {
            _state = MenuState::MAIN_MENU;
            spacePressed = true;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            spacePressed = false;
        }
    }
    else if (_state == MenuState::MAIN_MENU) {
        if (inputDelay.getElapsedTime().asSeconds() > 0.15f) {  // Add delay between inputs
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !returnPressed && _selectedOption == 0) {

            std::cout << "GameScene pointer: " << _gameScene << std::endl;
            std::cout << "GameScene entities: " << _gameScene->getEnts().size() << std::endl;


            // Add debug print
            std::cout << "Attempting scene transition" << std::endl;
            activeScene = _gameScene;  // Use global activeScene instead of _activeScene
            //_activeScene = _gameScene;
            returnPressed = true;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            returnPressed = false;
        }
    }

    // Update visibility
    for (auto& item : _menuItems) {
        item->setVisible(_state == MenuState::MAIN_MENU);
    }
}

void MenuComponent::addMenuItem(const std::shared_ptr<Entity>& item) {
    _menuItems.push_back(item);
}