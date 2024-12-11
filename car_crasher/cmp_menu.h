// cmp_menu.h
#pragma once
#include "ecm.h"
#include "scene.h"
#include <SFML/Graphics.hpp>
#include <vector>

class TextComponent : public Component {
protected:
    std::shared_ptr<sf::Font> _font;
    sf::Text _text;
    bool _selected;

public:
    explicit TextComponent(Entity* p, const std::string& str = "");
    void update(double dt) override;
    void render() override;

    void setText(const std::string& str);
    void setSelected(bool selected);
    void setCharacterSize(unsigned int size);
    void centerOrigin();
};

class MenuComponent : public Component {
public:  // Move enums to public section
    enum class MenuState { TITLE, MAIN_MENU, PAUSE_MENU };
    enum class MenuType { MAIN, PAUSE };

protected:
    MenuState _state;
    MenuType _type;
    size_t _selectedOption;
    std::vector<std::shared_ptr<Entity>> _menuItems;
    std::shared_ptr<Scene> _activeScene;
    std::shared_ptr<Scene> _gameScene;

public:
    explicit MenuComponent(Entity* p, MenuType type = MenuType::MAIN);

    void update(double dt) override;
    void render() override {}
    void addMenuItem(const std::shared_ptr<Entity>& item);
};