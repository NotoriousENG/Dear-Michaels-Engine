#include "UStaticModelComponent.h"
#include <Panels/Console.h>
#include "ResourceManagement/Mesh.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/Model.h"
#include "ResourceManagement/ResourceManager.h"
#include "Actors/AActor.h"
#include <Elements/Game.h>
#include <string>

UStaticModelComponent::UStaticModelComponent(std::shared_ptr<AActor> owner) : UComponent(owner)
{
	Init();
}

void UStaticModelComponent::Init()
{
	if (this->Shader == nullptr)
	{
		this->Shader = rm::ResourceManager::LoadShader(ShaderPaths[0].c_str(), ShaderPaths[1].c_str(), 
			nullptr, ShaderPaths[3].c_str());
	}
	if (this->Model == nullptr)
	{
		this->Model = rm::ResourceManager::LoadModel(ModelPath, false);
	}
}

void UStaticModelComponent::Draw(rm::Shader* shader)
{
	if (bDraw)
	{
		if (shader == nullptr)
		{
			shader = Shader.get();
		}
		shader->Use();
		shader->SetMatrix4("model", owner->model);
		shader->SetMatrix4("view", Game::MainCamera.view);
		shader->SetMatrix4("projection", Game::MainCamera.projection);

		Model->Draw(shader);
	}
}
