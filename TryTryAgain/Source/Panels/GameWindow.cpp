#include "GameWindow.h"

namespace Panels
{
	GameWindow::GameWindow()
	{
		ResourceManagement::ResourceManager::LoadShader("Assets/Shaders/Framebuffer.vert", "Assets/Shaders/Framebuffer.frag", nullptr, "Framebuffer");

		size = ImVec2(1920, 1080);

		// ---------------------------------Framebuffer begin:-----------------------------------------------

		// Allocate
		glGenFramebuffers(1, &framebuffer);

		glGenRenderbuffers(1, &rbo);

		glGenTextures(1, &texColorBuffer);

		init_framebuffer();

		// -----------------------------Framebuffer/Renderbuffer End ----------------------------------------

		MyGame = std::make_unique<Game>(framebuffer);

		ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
	}

	void GameWindow::Draw()
	{
		bool wasPlaying = playing;
		playing = MyGame->playing;

		ImGui::Begin("GameWindow: 1920 x 1080");
		{
			const char* playButtonLabel = playing ? "Press Escape to Exit Play Mode" : "Play (F5)";
			if (ImGui::Button(playButtonLabel))
			{
				playing = !playing;
			}
			// Using a Child allow to fill all the space of the window.
			// It also alows customization
			ImGui::BeginChild("Game");
			// Get the size of the child (i.e. the whole draw size of the windows).
			ImVec2 wsize = ImGui::GetWindowSize();
			Render(wsize);
			if (wsize.x != size.x || wsize.y != size.y)
			{
				size = wsize;
				init_framebuffer();
			}

			float windowAspect = size.x / size.y;
			float renderAspect = 1920.0f / 1080.0f;

			float scaleFactor;
			if (windowAspect > renderAspect)
			{
				scaleFactor = size.y / 1080.0f;
			}
			else
			{
				scaleFactor = size.x / 1920.0f;
			}
			if (playing && !wasPlaying)
			{
				MyGame->playing = true;
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}
			else if (!playing && wasPlaying)
			{
				MyGame->playing = false;
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}

			auto renderSize = ImVec2(1920 * scaleFactor, 1080 * scaleFactor);
			auto mousePos = ImGui::GetMousePos();
			auto renderPos = ImGui::GetWindowPos();

			auto gameMousePos = ImVec2(mousePos.x - renderPos.x, mousePos.y - renderPos.y);
			if (gameMousePos.x < 0 || gameMousePos.x > renderSize.x || gameMousePos.y < 0 || gameMousePos.y > renderSize.y)
			{
				gameMousePos.x = -1;
				gameMousePos.y = -1;
			}
			MyGame->mouse = glm::vec2(gameMousePos.x, gameMousePos.y);
			MyGame->renderSize = glm::vec2(renderSize.x, renderSize.y);


			// Because I use the texture from OpenGL, I need to invert the V from the UV.
			ImGui::Image((ImTextureID)texColorBuffer, renderSize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::EndChild();
		}
		ImGui::End();
	}

	void GameWindow::Render(ImVec2 size)
	{
		// Render Game to framebuffer
		MyGame->Render();

		// second pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 0, 0, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ResourceManagement::ResourceManager::GetShader("Framebuffer").Use();
		// glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		// glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void GameWindow::init_framebuffer()
	{
		glViewport(0, 0, size.x, size.y);

		// Update GL Render Properties
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// generate texture

		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

		// Renderbuffer begin:

		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			// M_LOG("[error]::FRAMEBUFFER:: Framebuffer is not complete!\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
