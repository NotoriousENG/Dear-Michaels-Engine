#pragma once 

#include <entt/entt.hpp>

#include "Modules/Editor/IEditor.h"
#include "Scene/Scene.h"

class NameComponent;

class SceneEditor : public IEditor
{
public:
    virtual void Draw() override;
    virtual void MenuItem() override;

    void ShowEntity(entt::entity entity, NameComponent* name = nullptr);

private:
    friend class Scene;
};
