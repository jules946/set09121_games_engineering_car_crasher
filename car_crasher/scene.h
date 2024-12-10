//scene.h
#pragma once
#include "entity_manager.h"

class Scene {
public:
    Scene() = default;

    virtual ~Scene() = default;
    virtual void update(double dt);
    virtual void render();
    virtual void load() = 0;
    std::vector<std::shared_ptr<Entity>> &getEnts();

protected:
    EntityManager _entity_manager;
};


extern std::shared_ptr<Scene> activeScene;
extern std::shared_ptr<Scene> menuScene;
extern std::shared_ptr<Scene> gameScene;
extern std::shared_ptr<Scene> pauseScene;
extern std::shared_ptr<Scene> gameOverScene;