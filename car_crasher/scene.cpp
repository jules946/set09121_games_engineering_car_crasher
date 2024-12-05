// scene.cpp

#include "scene.h"

// Load all entities in the scene
// Will be overridden per scene
void Scene::load() {}

// Update all entities in the scene
void Scene::update(const double dt) { _entity_manager.update(dt); }

// Render all entities in the scene
void Scene::render() { _entity_manager.render(); }