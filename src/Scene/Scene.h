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

    entt::registry registry;

private:
    

    friend class Entity;

#ifdef EDITOR
    friend class SceneEditor;
    friend class Inspector;
    entt::entity selectedEntity {0};
#endif //EDITOR
    friend class GL_RenderModule;
};