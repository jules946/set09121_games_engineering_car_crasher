// scene.cpp
#include "scene.h"

std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> menuScene;
std::shared_ptr<Scene> gameScene;
std::shared_ptr<Scene> pauseScene;

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