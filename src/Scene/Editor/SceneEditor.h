#pragma once 

#include <entt/entt.hpp>

#include "Scene/Scene.h"

class NameComponent;

class SceneEditor
{
public:
    void Draw();
    void MenuItem();

    void ShowEntity(entt::entity entity, NameComponent* name = nullptr);

    bool isActive;

private:
    friend class Scene;
};