#pragma once

#include <imgui.h>
#include <ImGuizmo.h>
#include "Serialization/ComponentFactory.h"

class Game; 
class UStaticModelComponent;

namespace Panels {
	class Inspector
	{
	public:

		Inspector(Game* game);

		ComponentFactory ComponentFactory;

		Game* MyGame;

		UStaticModelComponent* editingMC;

		void Draw();

		int gizmoCount = 1;

		bool useSnap = false;
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::WORLD;

	private:

		void EditTransform(float* cameraView, float* cameraProjection, float camDistance, float* matrix);

		void EditUStaticModelComponent(UStaticModelComponent* mc);

		void ShowFileDialog();
	};
}


