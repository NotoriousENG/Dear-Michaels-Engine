#include "ContentBrowser.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <Structs/FString.h>
#include "Elements/Game.h"
#include <Utility/Utility.h>
#include "ResourceManagement/Texture.h"

const std::filesystem::path Panels::ContentBrowser::assetPath = "Assets";

Panels::ContentBrowser::ContentBrowser()
{
}

void Panels::ContentBrowser::Draw()
{
	if (currentDirectory == "")
	{
		currentDirectory = assetPath;
	}
	ImGui::Begin("Content Browser");
	if (currentDirectory != std::filesystem::path(assetPath))
	{
		if (ImGui::Button("<-"))
		{
			currentDirectory = currentDirectory.parent_path();
		}
	}

	int c = 0;

	static float padding = 16;
	static float thumbnailSize = 128;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int maxColumns = (int)(panelWidth / cellSize);
	maxColumns = std::clamp(maxColumns, 1, 63);

	ImGui::SameLine();
	ImGui::Text(currentDirectory.string().c_str());

	int i = 0;

	if (ImGui::BeginTable(currentDirectory.string().c_str(), maxColumns))
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(currentDirectory))
		{
			ImGui::PushID(i++);
			if (c < maxColumns)
			{
				ImGui::TableNextColumn();
				c++;
			}
			else {
				ImGui::TableNextRow();
				c = 0;
			}

			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, assetPath);
			auto fileNameString = relativePath.filename().string();

			auto fileType = relativePath.extension().string();
			fileType = fileType.erase(0, 1);

			ImTextureID id;
			if (directoryEntry.is_directory()) {
				
				auto tp = rm::ResourceManager::Load<rm::Texture2D>("Resources/Icons/folder.png");
				IconTextures["Resources/Icons/folder.png"] = tp;
				id = (ImTextureID)tp->ID;
			}
			else {
				if (std::filesystem::exists(FString("Resources/Icons/%s.png", fileType.c_str()).Text))
				{
					auto tp = rm::ResourceManager::Load<rm::Texture2D>(FString("Resources/Icons/%s.png", fileType.c_str()).Text);
					IconTextures[FString("Resources/Icons/%s.png", fileType.c_str()).Text] = tp;
					id = (ImTextureID)tp->ID;
				}
				else
				{
					auto tp = rm::ResourceManager::Load<rm::Texture2D>("Resources/Icons/file.png");
					IconTextures["Resources/Icons/file.png"] = tp;
					id = (ImTextureID)tp->ID;
				}

			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			//ImGui::Button(fileNameString.c_str(), { thumbnailSize, thumbnailSize });

			ImGui::ImageButton(id, { thumbnailSize, thumbnailSize });
			
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				ImGui::BeginTooltip();
				ImGui::SetTooltip(path.string().c_str());
				ImGui::EndTooltip();
				std::string data = path.string();
				data = StringUtil::ReplaceAll(data, "\\", "/");
				ImGui::SetDragDropPayload("ASSET_PATH", data.c_str(), data.length() + 1);
				ImGui::EndDragDropSource();
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					currentDirectory /= path.filename();
				else if (fileType == "mscene")
					Game::instance->LoadScene(path.string().c_str());
			}
			ImGui::PopStyleColor();

			ImGui::TextWrapped(fileNameString.c_str());

			ImGui::PopID();
		}

		ImGui::EndTable();
	}
	

	ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	ImGui::SliderFloat("Padding", &padding, 0, 32);

	ImGui::End();
}

void Panels::ContentBrowser::MenuItem()
{
	ImGui::MenuItem("Content Browser", "", &isActive);
}
