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
protected:
    enum class MenuState { TITLE, MAIN_MENU };
    MenuState _state;
    size_t _selectedOption;
    std::vector<std::shared_ptr<Entity>> _menuItems;
    std::shared_ptr<Scene> _activeScene;    // Remove &
    std::shared_ptr<Scene> _gameScene;      // Remove &

    // std::shared_ptr<Scene>& _activeScene;    // Reference to active scene
    // std::shared_ptr<Scene>& _gameScene;      // Reference to game scene

public:
    explicit MenuComponent(Entity* p,
                          std::shared_ptr<Scene>& activeScene,
                          std::shared_ptr<Scene>& gameScene);
    void update(double dt) override;
    void render() override {}
    void addMenuItem(const std::shared_ptr<Entity>& item);
};