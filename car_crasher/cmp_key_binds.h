// cmp_key_binds.h
#pragma once
#include "ecm.h"
#include <SFML/Graphics.hpp>
extern std::map<sf::Keyboard::Key, std::string> keyToStringMap;

class KeyBindComponent : public Component {
protected:
    sf::Font _font;
    sf::Text _titleText;
    sf::Text _leftKeyText;
    sf::Text _rightKeyText;
    sf::Text _promptText;
    bool _waitingForLeft{false};
    bool _waitingForRight{false};
    static sf::Keyboard::Key _leftKey;
    static sf::Keyboard::Key _rightKey;

public:
    explicit KeyBindComponent(Entity* p);
    void update(double dt) override;
    void render() override;
    static sf::Keyboard::Key getLeftKey() { return _leftKey; }
    static sf::Keyboard::Key getRightKey() { return _rightKey; }
    static std::string keyToString(sf::Keyboard::Key key);
};