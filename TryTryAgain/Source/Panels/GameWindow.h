#pragma once
#include <cstdio>
#include <glad/glad.h>
#include <imgui.h>

#include "Elements/Game.h"
#include "Panels/Console.h"
#include "ResourceManagement/ResourceManager.h"

namespace Panels
{
	class GameWindow
	{
		
	public:
		
		GameWindow();

		void Draw();

		std::unique_ptr<Game> MyGame;

		bool playing;

	private:

		void Render(ImVec2 size);

		void init_framebuffer();

		unsigned int framebuffer;

		unsigned int texColorBuffer;

		unsigned int rbo;

		ImVec2 size;
		
	};
}
