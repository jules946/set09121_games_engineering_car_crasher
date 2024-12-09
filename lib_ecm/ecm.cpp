//ecm.cpp
#include "ecm.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

using namespace sf;
using namespace std;

// Entity class implementation

Entity::Entity()
    : _position(Vector2f(0.0f, 0.0f)), _rotation(0.0f), _alive(true), _visible(true), _fordeletion(false) {}

Entity::~Entity() = default;

void Entity::update(double dt) {
    if (!_alive) {
        return;
    }

    for (auto &comp : _components) {
        if (!comp->is_fordeletion()) {
            comp->update(dt);
        }
    }
    _components.erase(remove_if(_components.begin(), _components.end(),
                                [](const shared_ptr<Component> &comp) {
                                    return comp->is_fordeletion();
                                }),
                      _components.end());
}

void Entity::render() {
    if (!_visible)
        return;
    for (const auto &comp : _components) {
        comp->render();
    }
}

const Vector2f &Entity::getPosition() const {
    return _position;
}

void Entity::setPosition(const Vector2f &position) {
    _position = position;
}

bool Entity::is_fordeletion() const {
    return _fordeletion;
}

float Entity::getRotation() const {
    return _rotation;
}

void Entity::setRotation(float rotation) {
    _rotation = rotation;
}

bool Entity::isAlive() const {
    return _alive;
}

void Entity::setAlive(bool alive) {
    _alive = alive;
}

void Entity::setForDelete() {
    _fordeletion = true;
}

bool Entity::isVisible() const {
    return _visible;
}

void Entity::setVisible(bool visible) {
    _visible = visible;
}

// Component class implementation

Component::Component(Entity *const p) : _parent(p), _fordeletion(false) {}

Component::~Component() = default;

bool Component::is_fordeletion() const {
    return _fordeletion;
}
