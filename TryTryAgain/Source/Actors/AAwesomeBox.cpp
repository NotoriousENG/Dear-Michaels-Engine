#include "AAwesomeBox.h"

#include <glad/glad.h>

#include "Elements/Game.h"
#include "ResourceManagement/ResourceManager.h"
#include <Components/UStaticModelComponent.h>

AAwesomeBox::AAwesomeBox() : AActor()
{
	AAwesomeBox::Init();
}

void AAwesomeBox::Init()
{
	auto model_comp = AddComponent<UStaticModelComponent>();
	model_comp->Model = rm::ResourceManager::LoadModel("Assets/Models/backpack/backpack.obj", false, "Backpack");
	model_comp->Shader = rm::ResourceManager::LoadShader("Assets/Shaders/LoadModel.vert", "Assets/Shaders/LoadModel.frag", nullptr, "LoadModel");
}

void AAwesomeBox::Tick(float delta)
{
	Super::Tick(delta);

	transform.rotation = glm::rotate(transform.rotation, 12 * delta, glm::vec3(1, 0, 0));
}
