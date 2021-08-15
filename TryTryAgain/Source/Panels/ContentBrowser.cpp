#include "ContentBrowser.h"
#include <imgui.h>
#include <imgui_stdlib.h>

const std::filesystem::path Panels::ContentBrowser::assetPath = "Assets";

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

	ImGui::BeginTable(currentDirectory.string().c_str(), maxColumns);
	for (auto& directoryEntry : std::filesystem::directory_iterator(currentDirectory))
	{
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
		ImGui::Button(fileNameString.c_str(), { thumbnailSize, thumbnailSize });
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
				currentDirectory /= path.filename();
			
		}

		ImGui::Text(fileNameString.c_str());
	}

	ImGui::EndTable();

	ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	ImGui::SliderFloat("Padding", &padding, 0, 32);

	

	ImGui::End();
}

void Panels::ContentBrowser::MenuItem()
{
	ImGui::MenuItem("Content Browser", "", &isActive);
}
