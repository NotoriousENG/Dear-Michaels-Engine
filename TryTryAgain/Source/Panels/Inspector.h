#pragma once

#include <imgui.h>
#include <ImGuizmo.h>

class Game; 

namespace Panels {
	class Inspector
	{
	public:

		Inspector(Game* game) : MyGame(game) {};

		Game* MyGame;

		void Draw();

		int gizmoCount = 1;

		bool useSnap = false;
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::WORLD;

	private:

		void EditTransform(float* cameraView, float* cameraProjection, float camDistance, float* matrix, bool editTransformDecomposition);
	};
}


