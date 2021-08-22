#pragma once

#include "IPanel.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "Serialization/ComponentFactory.h"

class Game; 

namespace Panels {
	class Inspector : public IPanel
	{
	public:

		static UComponent* InspectedComponent;

		void Draw() override;

		void MenuItem() override;

		int gizmoCount = 1;

		bool useSnap = false;
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::WORLD;

	private:

		void EditTransform(float* cameraView, float* cameraProjection, float camDistance, float* matrix);

		void addComponentField();
	};
}


