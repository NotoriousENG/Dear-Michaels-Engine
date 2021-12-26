#pragma once

#include <string.h>

#include <entt/entt.hpp>

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();

    static Scene* Instance;

    Entity CreateEntity(const std::string& name = std::string());

    void OnUpdate(float delta);

private:
    entt::registry registry;

    friend class Entity;

#ifdef EDITOR
    friend class SceneEditor;
#endif //EDITOR
};