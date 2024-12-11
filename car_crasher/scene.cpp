// scene.cpp
#include "scene.h"

#include "cmp_key_binds.h"

std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> menuScene;
std::shared_ptr<Scene> gameScene;
std::shared_ptr<Scene> pauseScene;
std::shared_ptr<Scene> gameOverScene;
std::shared_ptr<Scene> keyBindScene;

// for testing menu
std::vector<std::shared_ptr<Entity>>& Scene::getEnts() {
    return _entity_manager.list;
}

// Load all entities in the scene
// Will be overridden per scene
void Scene::load() {}

// Update all entities in the scene
void Scene::update(const double dt) { _entity_manager.update(dt); }

// Render all entities in the scene
void Scene::render() { _entity_manager.render(); }

std::shared_ptr<Entity> Scene::makeEntity() {
    // Create a new entity
    auto entity = std::make_shared<Entity>();
    // Add it to the EntityManager
    _entity_manager.list.push_back(entity);
    // Return the created entity
    return entity;
}