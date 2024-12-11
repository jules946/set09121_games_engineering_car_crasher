// cmp_key_binds.cpp
#include "cmp_key_binds.h"
#include "game_config.h"
#include "system_renderer.h"
#include "scene.h"
#include <map>
#include <SFML/Window/Keyboard.hpp>

sf::Keyboard::Key KeyBindComponent::_leftKey = sf::Keyboard::Left;
sf::Keyboard::Key KeyBindComponent::_rightKey = sf::Keyboard::Right;

std::map<sf::Keyboard::Key, std::string> keyToStringMap = {
    {sf::Keyboard::A, "A"},
    {sf::Keyboard::B, "B"},
    {sf::Keyboard::C, "C"},
    {sf::Keyboard::D, "D"},
    {sf::Keyboard::E, "E"},
    {sf::Keyboard::F, "F"},
    {sf::Keyboard::G, "G"},
    {sf::Keyboard::H, "H"},
    {sf::Keyboard::I, "I"},
    {sf::Keyboard::J, "J"},
    {sf::Keyboard::K, "K"},
    {sf::Keyboard::L, "L"},
    {sf::Keyboard::M, "M"},
    {sf::Keyboard::N, "N"},
    {sf::Keyboard::O, "O"},
    {sf::Keyboard::P, "P"},
    {sf::Keyboard::Q, "Q"},
    {sf::Keyboard::R, "R"},
    {sf::Keyboard::S, "S"},
    {sf::Keyboard::T, "T"},
    {sf::Keyboard::U, "U"},
    {sf::Keyboard::V, "V"},
    {sf::Keyboard::W, "W"},
    {sf::Keyboard::X, "X"},
    {sf::Keyboard::Y, "Y"},
    {sf::Keyboard::Z, "Z"},
    {sf::Keyboard::Num0, "Num0"},
    {sf::Keyboard::Num1, "Num1"},
    {sf::Keyboard::Num2, "Num2"},
    {sf::Keyboard::Num3, "Num3"},
    {sf::Keyboard::Num4, "Num4"},
    {sf::Keyboard::Num5, "Num5"},
    {sf::Keyboard::Num6, "Num6"},
    {sf::Keyboard::Num7, "Num7"},
    {sf::Keyboard::Num8, "Num8"},
    {sf::Keyboard::Num9, "Num9"},
    {sf::Keyboard::Escape, "Escape"},
    {sf::Keyboard::LControl, "Left Control"},
    {sf::Keyboard::LShift, "Left Shift"},
    {sf::Keyboard::LAlt, "Left Alt"},
    {sf::Keyboard::LSystem, "Left System"},
    {sf::Keyboard::RControl, "Right Control"},
    {sf::Keyboard::RShift, "Right Shift"},
    {sf::Keyboard::RAlt, "Right Alt"},
    {sf::Keyboard::RSystem, "Right System"},
    {sf::Keyboard::Menu, "Menu"},
    {sf::Keyboard::LBracket, "Left Bracket"},
    {sf::Keyboard::RBracket, "Right Bracket"},
    {sf::Keyboard::Semicolon, "Semicolon"},
    {sf::Keyboard::Comma, "Comma"},
    {sf::Keyboard::Period, "Period"},
    {sf::Keyboard::Quote, "Quote"},
    {sf::Keyboard::Slash, "Slash"},
    {sf::Keyboard::Backslash, "Backslash"},
    {sf::Keyboard::Tilde, "Tilde"},
    {sf::Keyboard::Equal, "Equal"},
    {sf::Keyboard::Hyphen, "Hyphen"},
    {sf::Keyboard::Space, "Space"},
    {sf::Keyboard::Enter, "Enter"},
    {sf::Keyboard::Backspace, "Backspace"},
    {sf::Keyboard::Tab, "Tab"},
    {sf::Keyboard::PageUp, "Page Up"},
    {sf::Keyboard::PageDown, "Page Down"},
    {sf::Keyboard::End, "End"},
    {sf::Keyboard::Home, "Home"},
    {sf::Keyboard::Insert, "Insert"},
    {sf::Keyboard::Delete, "Delete"},
    {sf::Keyboard::Add, "Add"},
    {sf::Keyboard::Subtract, "Subtract"},
    {sf::Keyboard::Multiply, "Multiply"},
    {sf::Keyboard::Divide, "Divide"},
    {sf::Keyboard::Left, "Left Arrow"},
    {sf::Keyboard::Right, "Right Arrow"},
    {sf::Keyboard::Up, "Up Arrow"},
    {sf::Keyboard::Down, "Down Arrow"},
    {sf::Keyboard::Numpad0, "Numpad 0"},
    {sf::Keyboard::Numpad1, "Numpad 1"},
    {sf::Keyboard::Numpad2, "Numpad 2"},
    {sf::Keyboard::Numpad3, "Numpad 3"},
    {sf::Keyboard::Numpad4, "Numpad 4"},
    {sf::Keyboard::Numpad5, "Numpad 5"},
    {sf::Keyboard::Numpad6, "Numpad 6"},
    {sf::Keyboard::Numpad7, "Numpad 7"},
    {sf::Keyboard::Numpad8, "Numpad 8"},
    {sf::Keyboard::Numpad9, "Numpad 9"},
    {sf::Keyboard::F1, "F1"},
    {sf::Keyboard::F2, "F2"},
    {sf::Keyboard::F3, "F3"},
    {sf::Keyboard::F4, "F4"},
    {sf::Keyboard::F5, "F5"},
    {sf::Keyboard::F6, "F6"},
    {sf::Keyboard::F7, "F7"},
    {sf::Keyboard::F8, "F8"},
    {sf::Keyboard::F9, "F9"},
    {sf::Keyboard::F10, "F10"},
    {sf::Keyboard::F11, "F11"},
    {sf::Keyboard::F12, "F12"},
    {sf::Keyboard::Pause, "Pause"}
};


// Define keyToString as a member of KeyBindComponent
std::string KeyBindComponent::keyToString(sf::Keyboard::Key key) {
    if (keyToStringMap.count(key) > 0) {
        return keyToStringMap[key];
    } else {
        return "Unknown";
    }
}

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
    _leftKeyText.setString("Left Key: " + keyToString(_leftKey));
    _leftKeyText.setCharacterSize(32);
    _leftKeyText.setPosition(gameWidth / 2.f, gameHeight / 2.f - 50.f);
    _leftKeyText.setOrigin(_leftKeyText.getLocalBounds().width / 2.f,
                         _leftKeyText.getLocalBounds().height / 2.f);

    _rightKeyText.setFont(_font);
    _rightKeyText.setString("Right Key: " + keyToString(_rightKey));
    _rightKeyText.setCharacterSize(32);
    _rightKeyText.setPosition(gameWidth / 2.f, gameHeight / 2.f + 50.f);
    _rightKeyText.setOrigin(_rightKeyText.getLocalBounds().width / 2.f,
                          _rightKeyText.getLocalBounds().height / 2.f);

    _promptText.setFont(_font);
    _promptText.setString("Select key to change, ESC to return");
    _promptText.setCharacterSize(24);
    _promptText.setPosition(gameWidth / 2.f, gameHeight * 3.f / 4.f);
    _promptText.setOrigin(_promptText.getLocalBounds().width / 2.f,
                        _promptText.getLocalBounds().height / 2.f);
}


// Update function
void KeyBindComponent::update(double dt) {
    static int selectedOption = 0; // 0 for Left, 1 for Right
    static bool waitingForKey = false;

    // Track key state to detect single presses
    static bool upPressed = false;
    static bool downPressed = false;
    static bool enterPressed = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        activeScene = menuScene;
        return;
    }

    // Navigation logic (independent of waitingForKey)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if (!upPressed) {
            upPressed = true;
            selectedOption = (selectedOption == 0) ? 1 : 0; // Toggle between 0 and 1
        }
    } else {
        upPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (!downPressed) {
            downPressed = true;
            selectedOption = (selectedOption == 1) ? 0 : 1; // Toggle between 1 and 0
        }
    } else {
        downPressed = false;
    }

    // Handle waiting for key input

    if (waitingForKey) {
        for (int k = 0; k < sf::Keyboard::KeyCount; ++k) {
            auto key = static_cast<sf::Keyboard::Key>(k);
            if (sf::Keyboard::isKeyPressed(key)) {
                if (key == sf::Keyboard::Enter) continue; // Skip Enter

                // Update the selected key binding
                if (selectedOption == 0) {
                    _leftKey = key;
                    _leftKeyText.setString("Left Key: " + keyToString(_leftKey));
                } else if (selectedOption == 1) {
                    _rightKey = key;
                    _rightKeyText.setString("Right Key: " + keyToString(_rightKey));
                }
                waitingForKey = false; // Exit waiting state
                break;
            }
        }
    }


    // Handle Enter key to start waiting for key input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        if (!enterPressed) {
            enterPressed = true;
            waitingForKey = true;
            if (selectedOption == 0) {
                _leftKeyText.setString("Press new key for Left...");
            } else if (selectedOption == 1) {
                _rightKeyText.setString("Press new key for Right...");
            }
        }
    } else {
        enterPressed = false;
    }

    // Highlight the selected option
    if (selectedOption == 0) {
        _leftKeyText.setFillColor(sf::Color::Yellow);
        _rightKeyText.setFillColor(sf::Color::White);
    } else {
        _leftKeyText.setFillColor(sf::Color::White);
        _rightKeyText.setFillColor(sf::Color::Yellow);
    }
}

// Render function
void KeyBindComponent::render() {
    Renderer::queue(&_titleText);
    Renderer::queue(&_leftKeyText);
    Renderer::queue(&_rightKeyText);
    Renderer::queue(&_promptText);
}