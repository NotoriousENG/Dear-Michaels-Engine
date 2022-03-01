#include "Scene.h"
#include "Components.h"
#include <Entity.h>
#include "Input.h"
#include <SDL.h>

Scene* Scene::Instance = nullptr;

Scene::Scene() 
{
    Instance = this;
}

Scene::~Scene() 
{
    
}

void Scene::OnUpdate(float delta) 
{
    
}

Entity Scene::CreateEntity(const std::string& name) 
{
    Entity entity = {registry.create(), this};
    auto& nameC = entity.AddComponent<NameComponent>();
    nameC.Name = name.empty() ? "Entity" : name;
    return entity;
}
