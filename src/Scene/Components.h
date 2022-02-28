#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <ResourceManagement/Model.h>
#include "Scene/ComponentRegistry.h"

#include <Entity.h>

struct NameComponent
{
    std::string Name;
};

struct TransformComponent
{
    glm::mat4 transform = glm::mat4(1.0f);

    static void assign(Entity entity)
    {
        entity.AddComponent<TransformComponent>();
    }

    static void RegisterComponent()
    {
        auto name = "TransformComponent";
        auto f = entt::meta<TransformComponent>().type(entt::hashed_string(name))
            .data<&TransformComponent::transform>(entt::hashed_string("transform"))
            .func<&TransformComponent::assign>(entt::hashed_string("assign"));

        ComponentRegistry::registeredComponents[name] = entt::hashed_string(name);
    }
};

struct StaticMeshComponent
{
    StaticMeshComponent()
    {
        this->ModelPath = "Assets/Models/mario/mario-tex.obj";
        this->ShaderPath = "Assets/Shaders/LoadModel.glsl";
        this->Model = std::make_shared<rm::Model>();
        this->Model->Init(ModelPath);
    }
    std::shared_ptr<rm::Model> Model;
    std::string ModelPath = "Assets/Models/cube/cube.obj";
    std::string ShaderPath = "Assets/Shaders/LoadModel.glsl";
    bool bDraw = true;
};
