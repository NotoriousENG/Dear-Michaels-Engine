#include "UStaticModelComponent.h"
#include <Panels/Console.h>
#include "ResourceManagement/Mesh.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/Model.h"
#include "ResourceManagement/ResourceManager.h"
#include "Actors/AActor.h"
#include <Elements/Game.h>

UStaticModelComponent::UStaticModelComponent(AActor* owner) : UComponent(owner)
{
	
}

void UStaticModelComponent::Draw(rm::Shader* shader)
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

		Model->Draw(shader);
	}
}