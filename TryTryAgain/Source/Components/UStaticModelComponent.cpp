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
		this->Shader = rm::ResourceManager::LoadShader(ShaderPaths[0].c_str(), ShaderPaths[1].c_str(), 
			nullptr, ShaderPaths[3].c_str());
	}
	if (this->Model == nullptr)
	{
		this->Model = rm::ResourceManager::LoadModel(ModelPath, false);
	}
	this->name = "UStaticModelComponent";
}

bool UStaticModelComponent::ShowInspector()
{
	if (Super::ShowInspector())
	{
		if (ImGui::Button("Change Model"))
		{
			Panels::Inspector::InspectedComponent = this;
			ImGuiFileDialog::Instance()->OpenDialog("Load Model", "Load Model File", ".obj", "Assets/Models/");
		}
		ImGui::SameLine();
		ImGui::Text(this->Model->Path.c_str());

		if (Panels::Inspector::InspectedComponent == this)
		{
			showFileDialog();
		}
		else if (ImGuiFileDialog::Instance()->IsOpened("Load Model"))
		{
			ImGuiFileDialog::Instance()->Close();
		}

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
		shader->Use();
		shader->SetMatrix4("model", owner->model);
		shader->SetMatrix4("view", Game::MainCamera.view);
		shader->SetMatrix4("projection", Game::MainCamera.projection);

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

				this->Model = rm::ResourceManager::LoadModel(filePathName.c_str(), false);
				this->ModelPath = filePathName;
				Panels::Inspector::InspectedComponent = nullptr;
			}
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}
