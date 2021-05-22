#include "GameWindow.h"
#include <imguizmo/ImGuizmo.h>
#include <glm/gtx/matrix_decompose.hpp>

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

			ImVec2 p = ImGui::GetCursorScreenPos(); //TESTING
			
			// Because I use the texture from OpenGL, I need to invert the V from the UV.
			ImGui::Image((ImTextureID)texColorBuffer, renderSize, ImVec2(0, 1), ImVec2(1, 0));

			ImGui::EndChild();

			ImGui::BeginChild("Game");
			// ImGui::GetWindowDrawList()->AddLine(p, ImVec2(p.x + 50, p.y + 50), IM_COL32(255, 0, 0, 255), 3.0f);
			// Gizmos
			if (!MyGame->MouseButtons[SDL_BUTTON_RIGHT])
			{
				if (MyGame->Keys[SDLK_e])
					mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
				if (MyGame->Keys[SDLK_r])
					mCurrentGizmoOperation = ImGuizmo::ROTATE;
				if (MyGame->Keys[SDLK_t]) // r Key
					mCurrentGizmoOperation = ImGuizmo::SCALE;
			}
			

			if (!MyGame->Actors.empty() && MyGame->Picked != nullptr)
			{
				auto* SelectedActor = MyGame->Picked;

				ImGuizmo::SetOrthographic(false);

				ImGuizmo::SetDrawlist();
				
				float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
				float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				// Camera
				glm::mat4 view = MyGame->MainCamera.view;
				glm::mat4 proj = MyGame->MainCamera.projection;

				// Transform
				auto model = SelectedActor->model;
				auto& transform = SelectedActor->transform;

				if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), mCurrentGizmoOperation, ImGuizmo::WORLD, glm::value_ptr(model)));

				if (ImGuizmo::IsUsing())
				{
					SelectedActor->isUsing = true;
					SelectedActor->model = model;
					glm::decompose(model, transform.scale, transform.rotation, transform.position, transform.skew, transform.perspective);
				}
				else {
					SelectedActor->isUsing = false;
				}
			}

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

	void GameWindow::EditTransform(const Camera* camera, glm::mat4& matrix)
	{
		static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
		static bool boundSizing = false;
		static bool boundSizingSnap = false;

		if (MyGame->Keys[SDLK_e])
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (MyGame->Keys[SDLK_r])
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (MyGame->Keys[SDLK_t]) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(reinterpret_cast<float*>(&matrix), matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation);
		ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, reinterpret_cast<float*>(&matrix));

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("", &useSnap);
		ImGui::SameLine();
		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", snap);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", snap);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", snap);
			break;
		}
	}
}
