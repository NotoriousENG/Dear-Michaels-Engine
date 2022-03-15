#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <ResourceManagement/Model.h>
#include <ResourceManagement/ResourceManager.h>
#include "Scene/ComponentRegistry.h"
#include <ResourceManagement/Texture.h>

#include <Entity.h>

struct NameComponent
{
    std::string Name;

    static void Assign(Entity entity)
    {
        if (entity.HasComponent<NameComponent>())
        {
            return;
        }
        entity.AddComponent<NameComponent>();
    }

    static void RegisterComponent()
    {
        auto name = "NameComponent";
        auto f = entt::meta<NameComponent>().type(entt::hashed_string(name))
            .data<&NameComponent::Name>(entt::hashed_string("Name"))
            .func<&NameComponent::Assign>(entt::hashed_string("Assign"));

        ComponentRegistry::registeredComponents[name] = entt::hashed_string(name);
    }
};

struct TransformComponent
{
    glm::mat4 transform = glm::mat4(1.0f);

    static void Assign(Entity entity)
    {
        if (entity.HasComponent<TransformComponent>())
        {
            return;
        }
        entity.AddComponent<TransformComponent>();
    }

    static void RegisterComponent()
    {
        auto name = "TransformComponent";
        auto f = entt::meta<TransformComponent>().type(entt::hashed_string(name))
            .data<&TransformComponent::transform>(entt::hashed_string("transform"))
            .func<&TransformComponent::Assign>(entt::hashed_string("Assign"));

        ComponentRegistry::registeredComponents[name] = entt::hashed_string(name);
    }
};

struct StaticMeshComponent
{
    StaticMeshComponent()
    {
        this->ModelPath = "Assets/Models/mario/mario-tex.obj";
        this->ShaderPath = "Assets/Shaders/LoadModel.glsl";
        this->Model = rm::ResourceManager::Load<rm::Model>("Assets/Models/mario/mario-tex.obj");
        this->Model->Init(ModelPath);
    }
    std::shared_ptr<rm::Model> Model;
    std::string ModelPath = "Assets/Models/cube/cube.obj";
    std::string ShaderPath = "Assets/Shaders/LoadModel.glsl";
    bool bDraw = true;

    static void Assign(Entity entity)
    {
        if (entity.HasComponent<StaticMeshComponent>())
        {
            return;
        }
        entity.AddComponent<StaticMeshComponent>();
    }

    static void RegisterComponent()
    {
        auto name = "StaticMeshComponent";
        auto f = entt::meta<StaticMeshComponent>().type(entt::hashed_string(name))
            .data<&StaticMeshComponent::ModelPath>(entt::hashed_string("ModelPath"))
            .data<&StaticMeshComponent::ShaderPath>(entt::hashed_string("ShaderPath"))
            .func<&StaticMeshComponent::Assign>(entt::hashed_string("Assign"));

        ComponentRegistry::registeredComponents[name] = entt::hashed_string(name);
    }
};

struct SpriteComponent
{
    SpriteComponent()
    {
        this->TexturePath = "Assets/Textures/awesomeface.png";
        this->Sprite = rm::ResourceManager::Load<rm::Texture2D>(this->TexturePath);
    }
    std::shared_ptr<rm::Texture2D> Sprite;
    std::string TexturePath = "Assets/Textures/awesomeface.png";

    static void Assign(Entity entity)
    {
        if (entity.HasComponent<SpriteComponent>())
        {
            return;
        }
        entity.AddComponent<SpriteComponent>();
    }

    static void RegisterComponent()
    {
        auto name = "SpriteComponent";
        auto f = entt::meta<SpriteComponent>().type(entt::hashed_string(name))
            .data<&SpriteComponent::TexturePath>(entt::hashed_string("TexturePath"))
            .func<&SpriteComponent::Assign>(entt::hashed_string("Assign"));

        ComponentRegistry::registeredComponents[name] = entt::hashed_string(name);
    }
};

struct MovementComponent
{
    float speed = 5.0f;

    static void Assign(Entity entity)
    {
        if (entity.HasComponent<MovementComponent>())
        {
            return;
        }
        entity.AddComponent<MovementComponent>();
    }

    static void RegisterComponent()
    {
        auto name = "MovementComponent";
        auto f = entt::meta<MovementComponent>().type(entt::hashed_string(name))
            .data<&MovementComponent::speed>(entt::hashed_string("speed"))
            .func<&MovementComponent::Assign>(entt::hashed_string("Assign"));

        ComponentRegistry::registeredComponents[name] = entt::hashed_string(name);
    }
};
