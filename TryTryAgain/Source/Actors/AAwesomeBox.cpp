#include "AAwesomeBox.h"

#include <glad/glad.h>

#include "Elements/Game.h"
#include "ResourceManagement/ResourceManager.h"
#include <Components/UStaticMeshComponent.h>

AAwesomeBox::AAwesomeBox(const char* name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : AActor(name, pos, rot, scale)
{
	rm::ResourceManager::LoadTexture("Assets/Textures/container.jpg", false, "container");
	rm::ResourceManager::LoadTexture("Assets/Textures/awesomeface.png", true, "awesomeface");

	auto mesh = rm::ResourceManager::CreateMesh("Box");
	auto mesh_comp = AddComponent<UStaticMeshComponent>();
	mesh_comp->Mesh = mesh;
	mesh_comp->Shader = rm::ResourceManager::LoadShader("Assets/Shaders/Lit.vert", "Assets/Shaders/Standard.frag", nullptr, "Standard");
}

void AAwesomeBox::Tick(float delta)
{
	Super::Tick(delta);

	transform.rotation = glm::rotate(transform.rotation, 1 * delta, glm::vec3(1, 0, 0));
}
