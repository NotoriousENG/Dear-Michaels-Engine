#include "Scene.h"
#include "Components.h"
#include <Entity.h>
#include "Input.h"
#include <SDL.h>
#include <Elements/Camera.h>
#include <Systems/Systems.h>

#ifdef EDITOR
#include <EditorModule.h>
#endif // EDITOR

Scene* Scene::Instance = nullptr;

Scene::Scene() 
{
    Instance = this;

    {
        auto e = this->CreateEntity("Smile");
        e.AddComponent<TransformComponent>();
        e.AddComponent<SpriteComponent>();
        e.AddComponent<MovementComponent>();
    }

}

Scene::~Scene() 
{
    
}

void Scene::OnUpdate(float delta) 
{
    bool isPlaying = true;

#ifdef EDITOR
    isPlaying = EditorModule::playing;
#endif // EDITOR

    if (Input::MouseButtons[3] && !isPlaying)
    {
        Camera::Main.ProcessMouseMovement(Input::MouseRel.x, -Input::MouseRel.y);

        auto inputVec = glm::vec3(Input::GetAxisRight(), Input::GetAxisUp(), -Input::GetAxisForward());
        Camera::Main.ProcessKeyboard(inputVec, .01f);
    }

    glm::vec3 moveDir = glm::vec3(Input::GetAxisRight(), Input::GetAxisForward(), 0);

    if (isPlaying)
    {
        auto view = Scene::Instance->registry.view<TransformComponent, MovementComponent>();

        // use forward iterators and get only the components of interest
        for (auto entity : view)
        {
            auto& [transform, move] = view.get<TransformComponent, MovementComponent>(entity);

            PlayerMove(transform, move, moveDir, delta);
        }
    }
}

Entity Scene::CreateEntity(const std::string& name) 
{
    Entity entity = {registry.create(), this};
    auto& nameC = entity.AddComponent<NameComponent>();
    nameC.Name = name.empty() ? "Entity" : name;
    return entity;
}
