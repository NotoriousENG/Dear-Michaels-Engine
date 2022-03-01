#pragma once

#include "Scene.h"
#include <entt/entt.hpp>

#include "EngineAssert.h"

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        ENGINE_ASSERT(!HasComponent<T>(), "Entity already has component");
        return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
    }

    template<typename T>
    T& GetComponent()
    {
        ENGINE_ASSERT(HasComponent<T>(), "Entity does not have component");
        return scene->registry.get<T>(entityHandle);
    }

    template<typename T>
    bool HasComponent()
    {
        return scene->registry.any_of<T>(entityHandle);
    }

    template<typename T>
    void RemoveComponent()
    {
        scene->registry.remove<T>(entityHandle);
    }

    void DestroyEntity()
    {
        scene->registry.destroy(entityHandle);
#ifdef EDITOR
        if (*Scene::Instance->selectedEntity == entityHandle)
        {
            delete(scene->Instance->selectedEntity);
            scene->Instance->selectedEntity = nullptr;
        }
#endif // EDITOR
    }

    operator bool() const {return (int)entityHandle != 0; }

    entt::entity GetHandle();

private:
    entt::entity entityHandle { };
    Scene* scene = nullptr;

    friend class Scene;
};