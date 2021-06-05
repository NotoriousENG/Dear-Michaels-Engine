#include "UStaticMeshComponent.h"
#include <Panels/Console.h>
#include "ResourceManagement/Mesh.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/ResourceManager.h"
#include "Actors/AActor.h"
#include <Elements/Game.h>

UStaticMeshComponent::UStaticMeshComponent(AActor* owner) : UComponent(owner)
{

}

void UStaticMeshComponent::Tick(float delta)
{
	// M_LOG("Poop");
}

void UStaticMeshComponent::Draw(rm::Shader* shader)
{
	if (bDraw)
	{
		if (shader == nullptr)
		{
			shader = Shader;
		}
		shader->Use();
		shader->SetMatrix4("model", owner->model);
		shader->SetMatrix4("view", Game::MainCamera.view);
		shader->SetMatrix4("projection", Game::MainCamera.projection);

		glActiveTexture(GL_TEXTURE0);
		rm::ResourceManager::GetTexture("container")->Bind();
		glActiveTexture(GL_TEXTURE1);
		rm::ResourceManager::GetTexture("awesomeface")->Bind();

		rm::ResourceManager::GetShader("Standard")->SetInteger("texture1", 0);
		rm::ResourceManager::GetShader("Standard")->SetInteger("texture2", 1);

		Mesh->Draw(shader);
	}
}

