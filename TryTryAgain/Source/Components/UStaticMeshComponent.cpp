#include "UStaticMeshComponent.h"
#include <ResourceManagement/ResourceManager.h>
#include "ResourceManagement/Mesh.h"
#include "ResourceManagement/Material.h"
#include "ResourceManagement/Shader.h"
#include "Actors/AActor.h"
#include <Elements/Camera.h>
#include <Panels/Inspector.h>
#include <ResourceManagement/Model.h>

UStaticMeshComponent::UStaticMeshComponent(std::shared_ptr<AActor> owner) : UComponent(owner)
{
}

void UStaticMeshComponent::Init()
{
	if (this->Mesh == nullptr)
	{
		this->Mesh = std::make_unique<rm::Mesh>(rm::ResourceManager::Load<rm::Model>("Assets/Models/cube/cube.obj")->meshes[0]);
		this->Material = std::make_shared<rm::Material>();

		this->Material->Shader = rm::ResourceManager::Load<rm::Shader>("Assets/Shaders/LoadModel.glsl");
		// this->Material->Textures = this->Mesh->textures;
		this->Material->InitUniforms();

		this->Material->name = "DefaultMaterial";
	}
	this->name = "UStaticMeshComponent";
}

void UStaticMeshComponent::Draw(DRAW_TYPE type)
{
	if (bDraw)
	{
		std::shared_ptr<rm::Shader> shader;
		switch (type)
		{
		case DRAW_TYPE::MATERIAL:
			shader = Material->Shader;
			break;
		case DRAW_TYPE::PICKING:
			shader = rm::ResourceManager::Load<rm::Shader>("Assets/Shaders/Picking.glsl");
			break;
		default:
			shader = Material->Shader;
			break;
		}

		if (shader)
		{
			shader->Use();
			shader->SetMatrix4("model", owner->transform->GetModelMatrixWorld());
			shader->SetMatrix4("view", Camera::Main.view);
			shader->SetMatrix4("projection", Camera::Main.projection);

			Mesh->Draw(shader.get());
		}
	}
}

bool UStaticMeshComponent::ShowInspector()
{
	if (Super::ShowInspector())
	{
		if (ImGui::Button("Model:"))
		{
			Panels::Inspector::InspectedComponent = this;
			// ImGuiFileDialog::Instance()->OpenDialog("Load Model", "Load Model File", ".obj", "Assets/Models/");
		}

		float margin = 2;

		ImGui::SameLine();
		auto pos = ImGui::GetCursorScreenPos();
		ImGui::PushTextWrapPos();
		ImGui::Text("Assets/Models/cube/cube.obj");
		ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImColor(1.0f, 1.0f, 1.0f, 0.25f));
		ImGui::PopTextWrapPos();

		if (Mesh)
		{
			Mesh->material->ShowInspector();
		}

		return true;
	}

	return false;
}
