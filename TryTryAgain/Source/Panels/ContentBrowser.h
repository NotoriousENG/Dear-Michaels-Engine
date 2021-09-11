#pragma once
#include "IPanel.h"
#include <filesystem>
#include <ResourceManagement/ResourceManager.h>
#include <unordered_map>
namespace Panels
{
	class ContentBrowser : public IPanel
	{
	public:

		ContentBrowser();

		void Draw() override;

		void MenuItem() override;

	private:

		std::filesystem::path currentDirectory;

		static const std::filesystem::path assetPath;

		std::unordered_map<std::string, std::shared_ptr<rm::Texture2D>> IconTextures;
	};
}


