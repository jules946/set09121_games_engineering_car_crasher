//entity.cpp
#include "entity_manager.h"

#include "cmp_sprite.h"
#include "system_renderer.h"
using namespace std;
using namespace sf;

void EntityManager::removeDeletedEntities() {
    list.erase(std::remove_if(list.begin(), list.end(),
                              [](const std::shared_ptr<Entity>& entity) { return entity->is_fordeletion(); }),
               list.end());
}

void EntityManager::update(const double dt) {
    for (const auto &entity : list) {
        entity->update(dt);
    }
    removeDeletedEntities();
}

// Add all entities to the renderer queue
void EntityManager::render() const {
    for (const auto &entity : list) {
        entity->render();
    }
}
