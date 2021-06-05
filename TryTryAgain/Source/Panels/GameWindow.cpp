#include "GameWindow.h"
#include <imguizmo/ImGuizmo.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "imgui_stdlib.h"

namespace Panels
{
	GameWindow::GameWindow()
	{
		M_LOG("# Select Actors by clicking on them");
		M_LOG("# Press W, E, R, T to toggle between Translate, Rotate, Scale, and Bound Tools");
		M_LOG("# You can also delete selected actors by pressing the delete key");

		rm::ResourceManager::LoadShader("Assets/Shaders/Framebuffer.vert", "Assets/Shaders/Framebuffer.frag", nullptr, "Framebuffer");

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

		ImGui::Begin("GameWindow");
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

			if (wsize.x != size.x || wsize.y != size.y)
			{
				MyGame->MainCamera.AspectRatio = (wsize.x / 800) / (wsize.y / 800);
				MyGame->MainCamera.UpdateCameraVectors();
				size = wsize;
				init_framebuffer();
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

			auto mousePos = ImGui::GetMousePos();
			auto renderPos = ImGui::GetWindowPos();

			auto gameMousePos = ImVec2(mousePos.x - renderPos.x, mousePos.y - renderPos.y);
			if (gameMousePos.x < 0 || gameMousePos.x > wsize.x || gameMousePos.y < 0 || gameMousePos.y > wsize.y)
			{
				gameMousePos.x = -1;
				gameMousePos.y = -1;
			}
			MyGame->mouse = glm::vec2(gameMousePos.x, gameMousePos.y);
			MyGame->renderSize = glm::vec2(wsize.x, wsize.y);

			ImVec2 p = ImGui::GetCursorScreenPos(); //TESTING
			
			// Because I use the texture from OpenGL, I need to invert the V from the UV.
			ImGui::Image((ImTextureID)texColorBuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));

			// ImGui::GetWindowDrawList()->AddLine(p, ImVec2(p.x + 50, p.y + 50), IM_COL32(255, 0, 0, 255), 3.0f);
			// Gizmos
			if (!MyGame->MouseButtons[SDL_BUTTON_RIGHT])
			{
				if (MyGame->Keys[SDLK_w])
					mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
				if (MyGame->Keys[SDLK_e])
					mCurrentGizmoOperation = ImGuizmo::ROTATE;
				if (MyGame->Keys[SDLK_r]) // r Key
					mCurrentGizmoOperation = ImGuizmo::SCALE;
				if (MyGame->Keys[SDLK_t])
					mCurrentGizmoOperation = ImGuizmo::BOUNDS;
			}

			float* model_arr = glm::value_ptr(MyGame->Picked->model);
			auto& camera = MyGame->MainCamera;

			if (!MyGame->Actors.empty() && MyGame->Picked != nullptr)
			{

				ImGuizmo::SetGizmoSizeClipSpace(0.2);

				EditTransform(glm::value_ptr(camera.view), glm::value_ptr(camera.projection), glm::distance(glm::normalize(camera.transform.position), glm::vec3(0)), model_arr, true);

				auto& pt =  MyGame->Picked->transform;
				glm::decompose(MyGame->Picked->model, pt.scale, pt.rotation, pt.position, pt.skew, pt.perspective);
			}

			Render();

			ImGui::EndChild();
			
		}
		ImGui::End();
	}

	void GameWindow::Render()
	{
		// Render Game to framebuffer
		MyGame->Render();
		
		// second pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 0, 0, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rm::ResourceManager::GetShader("Framebuffer").Use();
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

	void GameWindow::EditTransform(float* cameraView, float* cameraProjection, float camDistance, float* matrix, bool editTransformDecomposition)
	{
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
		static bool b_useSnap = false;
		static float snap[3] = { 1.f, 1.f, 1.f };
		static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
		static bool boundSizing = false;
		static bool boundSizingSnap = false;

		static bool allowAxisFlip = true;

		if (editTransformDecomposition)
		{
			ImGui::Begin("Inspector");

			auto actor = MyGame->Picked;
			ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2);
			ImGui::InputText("", &actor->name);
			ImGui::PopItemWidth();
			ImGui::TableSetColumnIndex(1);
			int uid = reinterpret_cast<unsigned int>(actor);
			ImGui::Text("ID: 0x%x", uid);
			ImGui::EndTable();

			if (ImGui::IsKeyPressed(90))
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (ImGui::IsKeyPressed(69))
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			if (ImGui::IsKeyPressed(82)) // r Key
				mCurrentGizmoOperation = ImGuizmo::SCALE;
			if (ImGui::RadioButton("Tr", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Rt", mCurrentGizmoOperation == ImGuizmo::ROTATE))
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Sc", mCurrentGizmoOperation == ImGuizmo::SCALE))
				mCurrentGizmoOperation = ImGuizmo::SCALE;

			ImGui::SameLine();
			if (ImGui::RadioButton("Bd", mCurrentGizmoOperation == ImGuizmo::BOUNDS))
			{
				mCurrentGizmoOperation = ImGuizmo::BOUNDS;
			}

			if (mCurrentGizmoOperation == ImGuizmo::SCALE)
			{
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			}

			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
			ImGui::InputFloat3("Tr", matrixTranslation);
			ImGui::InputFloat3("Rt", matrixRotation);
			ImGui::InputFloat3("Sc", matrixScale);
			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

			if (mCurrentGizmoOperation == ImGuizmo::BOUNDS)
			{
				ImGui::PushID(3);
				ImGui::Checkbox("Snap", &boundSizingSnap);
				ImGui::SameLine();
				ImGui::InputFloat3("Bounds", boundsSnap);
				ImGui::PopID();
			}
			boundSizing = mCurrentGizmoOperation == ImGuizmo::BOUNDS;

			if (mCurrentGizmoOperation != ImGuizmo::SCALE && mCurrentGizmoOperation != ImGuizmo::BOUNDS)
			{
				if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
					mCurrentGizmoMode = ImGuizmo::LOCAL;
				ImGui::SameLine();
				if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
					mCurrentGizmoMode = ImGuizmo::WORLD;
			}
			if (ImGui::IsKeyPressed(83))
				b_useSnap = !b_useSnap;
			if (!boundSizing)
			{
				if (ImGui::Checkbox("Snap", &b_useSnap));
				ImGui::SameLine();
			}

			switch (mCurrentGizmoOperation)
			{
			case ImGuizmo::TRANSLATE:
				ImGui::InputFloat3("Pos", &snap[0]);
				break;
			case ImGuizmo::ROTATE:
				ImGui::InputFloat("Rot", &snap[0]);
				break;
			case ImGuizmo::SCALE:
				ImGui::InputFloat("Scale", &snap[0]);
				break;
			}
			
			ImGui::Checkbox("Allow Axis Flip", &allowAxisFlip);
			ImGuizmo::AllowAxisFlip(allowAxisFlip);

			ImGui::End();
		}

		ImGuizmo::SetDrawlist();

		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		auto viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
		auto viewManipulateTop = ImGui::GetWindowPos().y;

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);

		if (!MyGame->playing)
			ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, b_useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
		
		// ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
		// ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

		if (false)
		{
			//ImGui::End();
			ImGui::PopStyleColor(1);
		}
	}
}
