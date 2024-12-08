// entity_manager.h
#pragma once

#include <memory>
#include "../lib_ecm/ecm.h"


struct EntityManager {
    // List of entities
    std::vector<std::shared_ptr<Entity>> list;

    void removeDeletedEntities();

    // Update all entities
    void update(double dt);
    // Add all entities to the renderer queue
    void render() const;
};