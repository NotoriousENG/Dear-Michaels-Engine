#pragma once
#include <cstdio>
#include <glad/glad.h>
#include <imgui.h>
#include <ImGuizmo.h>


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

		void Render();

		void init_framebuffer();

		unsigned int framebuffer;

		unsigned int texColorBuffer;

		unsigned int rbo;

		ImVec2 size;

		float snap[3] = { 1.f, 1.f, 1.f };

		const float identityMatrix[16] =
		{ 1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f };

		float objectMatrix[4][16] = {
		  { 1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f },

		  { 1.f, 0.f, 0.f, 0.f,
		  0.f, 1.f, 0.f, 0.f,
		  0.f, 0.f, 1.f, 0.f,
		  2.f, 0.f, 0.f, 1.f },

		  { 1.f, 0.f, 0.f, 0.f,
		  0.f, 1.f, 0.f, 0.f,
		  0.f, 0.f, 1.f, 0.f,
		  2.f, 0.f, 2.f, 1.f },

		  { 1.f, 0.f, 0.f, 0.f,
		  0.f, 1.f, 0.f, 0.f,
		  0.f, 0.f, 1.f, 0.f,
		  0.f, 0.f, 2.f, 1.f }
		};

		int gizmoCount = 1;

		bool useSnap = false;
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::WORLD;

		void EditTransform(float* cameraView, float* cameraProjection, float camDistance, float* matrix, bool editTransformDecomposition);

	};
}
