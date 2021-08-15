#pragma once
#include "IPanel.h"
#include <filesystem>
namespace Panels
{
	class ContentBrowser : public IPanel
	{
	public:

		void Draw() override;

		void MenuItem() override;

	private:

		std::filesystem::path currentDirectory;

		static const std::filesystem::path assetPath;
	};
}


