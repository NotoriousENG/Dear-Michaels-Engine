#pragma once

#include "UStaticModelComponent.h"
#include <Panels/Console.h>
#include "ResourceManagement/Mesh.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/Model.h"
#include "ResourceManagement/ResourceManager.h"
#include "Actors/AActor.h"
#include <Elements/Game.h>
#include <string>

// EDITOR
#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <Panels/Inspector.h>
#include <Utility/Utility.h>
// EDITOR


UStaticModelComponent::UStaticModelComponent(std::shared_ptr<AActor> owner) : UComponent(owner)
{
	Init();
}

void UStaticModelComponent::Init()
{
	if (this->Shader == nullptr)
	{
		this->Shader = rm::ResourceManager::Load<rm::Shader>(ShaderPath);
	}
	if (this->Model == nullptr)
	{
		this->Model = rm::ResourceManager::Load<rm::Model>(ModelPath);
	}
	this->name = "UStaticModelComponent";

	this->Material = std::make_shared<rm::Material>();

	Material->Shader = this->Shader;

	Material->InitUniforms();
}

bool UStaticModelComponent::ShowInspector()
{
	if (Super::ShowInspector())
	{
		if (ImGui::Button("Model:"))
		{
			Panels::Inspector::InspectedComponent = this;
			ImGuiFileDialog::Instance()->OpenDialog("Load Model", "Load Model File", ".obj", "Assets/Models/");
		}

		float margin = 2;

		ImGui::SameLine();
		auto pos = ImGui::GetCursorScreenPos();
		ImGui::PushTextWrapPos();
		ImGui::Text(this->Model->Path.c_str());
		ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImColor(1.0f, 1.0f, 1.0f, 0.25f));
		ImGui::PopTextWrapPos();

		if (ImGui::BeginDragDropTarget())
		{
			auto payload = ImGui::AcceptDragDropPayload("ASSET_PATH");
			if (payload != nullptr)
			{
				std::string data = (const char*)payload->Data;
				if (data.find(".obj") != std::string::npos) {
					this->Model = rm::ResourceManager::Load<rm::Model>(data.c_str());
					
					this->ModelPath = data;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (Panels::Inspector::InspectedComponent == this)
		{
			showFileDialog();
		}
		else if (ImGuiFileDialog::Instance()->IsOpened("Load Model"))
		{
			ImGuiFileDialog::Instance()->Close();
		}

		Material->ShowInspector();

		return true;
	}

	return false;
}

void UStaticModelComponent::Draw(rm::Shader* shader)
{
	if (bDraw)
	{
		if (shader == nullptr)
		{
			shader = Shader.get();
		}
		Material->SetUniforms();
		shader->Use();
		shader->SetMatrix4("model", owner->model);
		shader->SetMatrix4("view", Camera::Main.view);
		shader->SetMatrix4("projection", Camera::Main.projection);

		Model->Draw(shader);
	}
}

void UStaticModelComponent::showFileDialog()
{
	// display
	if (ImGuiFileDialog::Instance()->Display("Load Model"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			
			if (Panels::Inspector::InspectedComponent == this)
			{
				int localIndex = filePathName.find("Assets");
				if (localIndex != string::npos)
				{
					filePathName = filePathName.substr(localIndex);
				}

				filePathName = StringUtil::ReplaceAll(filePathName, "\\", "/");

				this->Model = rm::ResourceManager::Load<rm::Model>(filePathName.c_str());
				
				this->ModelPath = filePathName;
				Panels::Inspector::InspectedComponent = nullptr;
			}
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}
