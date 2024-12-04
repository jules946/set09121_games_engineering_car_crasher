//"ecm.h"
#pragma once
#include "maths.h"
#include <memory>
#include <vector>

class Component; //forward declare

class Entity {

protected:
    std::vector<std::shared_ptr<Component>> _components;
    sf::Vector2f _position;
    float _rotation;
    bool _alive;       // should be updated
    bool _visible;     // should be rendered
    bool _fordeletion; // should be deleted
public:
    Entity();
    virtual ~Entity();
    virtual void update(double dt);
    virtual void render();

    const sf::Vector2f &getPosition() const;
    void setPosition(const sf::Vector2f &_position);
    bool is_fordeletion() const;
    float getRotation() const;
    void setRotation(float _rotation);
    bool isAlive() const;
    void setAlive(bool _alive);
    void setForDelete();
    bool isVisible() const;
    void setVisible(bool _visible);

    template <typename T, typename... Targs>
    std::shared_ptr<T> addComponent(Targs... params) {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
        _components.push_back(sp);
        return sp;
    }
    template <typename T>
    const std::vector<std::shared_ptr<T>> get_components() const {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::vector<std::shared_ptr<T>> ret;
        for (const auto c : _components) {
            if (typeid(*c) == typeid(T)) {
                ret.push_back(std::dynamic_pointer_cast<T>(c));
            }
        }
        return std::move(ret);
    }
    template <typename T>
    const std::vector<std::shared_ptr<T>> GetCompatibleComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::vector<std::shared_ptr<T>> ret;
        for (auto c : _components) {
            auto dd = dynamic_cast<T*>(&(*c));
            if (dd) {
                ret.push_back(std::dynamic_pointer_cast<T>(c));
            }
        }
        return ret;
    }
};

class Component {
protected:
    Entity *const _parent;
    bool _fordeletion; // should be removed
    explicit Component(Entity *p);

public:
    Component() = delete;
    bool is_fordeletion() const;
    virtual void update(double dt) = 0;
    virtual void render() = 0;
    virtual ~Component();
};

