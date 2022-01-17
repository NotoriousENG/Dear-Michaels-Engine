#include "Inspector.h"
#include <Entity.h>
#include <Scene/Components.h>
#include <Elements/Camera.h>
#include <ImGuiFileDialog.h>
#include <ResourceManagement/ResourceManager.h>
#include <Utility/Utility.h>

void Inspector::Draw() 
{
	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	ImGui::Begin("Inspector");

	ShowComponents();

	ImGui::End();
}
void Inspector::MenuItem() 
{
	ImGui::MenuItem("Inspector", "", &bDrawable);
}

void Inspector::ShowComponents()
{
	auto entity = Entity(Scene::Instance->selectedEntity, Scene::Instance);

	if (entity.HasComponent<NameComponent>())
	{
		auto& name = entity.GetComponent<NameComponent>();

		ShowComponentHeader("Name");

		ImGui::InputText("Name", &name.Name);
	}

	if (entity.HasComponent<TransformComponent>())
	{
		auto transform = glm::value_ptr(entity.GetComponent<TransformComponent>().transform);
		auto& camera = Camera::Main;
		bool manipulated = false;

		ShowComponentHeader("Transform");
		EditTransform(glm::value_ptr(camera.view), glm::value_ptr(camera.projection), glm::distance(glm::normalize(camera.position), glm::vec3(0)), transform, transform, manipulated);
	}

	if (entity.HasComponent<StaticMeshComponent>())
	{
		auto& mesh = entity.GetComponent<StaticMeshComponent>();

		ShowComponentHeader("Mesh");

		if (ImGui::Button("Model:"))
		{
			ImGuiFileDialog::Instance()->OpenDialog("Load Model", "Load Model File", ".obj", "Assets/Models/");
			fileDialogOpen = true;
		}

		float margin = 2;

		ImGui::SameLine();
		auto pos = ImGui::GetCursorScreenPos();
		ImGui::PushTextWrapPos();
		ImGui::Text(mesh.Model->Path.c_str());
		ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImColor(1.0f, 1.0f, 1.0f, 0.25f));
		ImGui::PopTextWrapPos();

		if (ImGui::BeginDragDropTarget())
		{
			auto payload = ImGui::AcceptDragDropPayload("ASSET_PATH");
			if (payload != nullptr)
			{
				std::string data = (const char*)payload->Data;
				if (data.find(".obj") != std::string::npos) {
					mesh.Model = rm::ResourceManager::Load<rm::Model>(data.c_str());

					mesh.ModelPath = data;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (fileDialogOpen) {
			showFileDialog(mesh);
		}
		else if (ImGuiFileDialog::Instance()->IsOpened("Load Model"))
		{
			ImGuiFileDialog::Instance()->Close();
		}
	}
}

void Inspector::ShowComponentHeader(const char* title)
{
	ImGui::Separator();

	if (ImGui::BeginTable("split", 2, ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableNextColumn();
		ImGui::Text(title);

		ImGui::TableNextColumn();
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 1));
		if (ImGui::Button("X"))
		{
			ImGui::PopStyleColor();
			ImGui::EndTable();

			return;
		}
		ImGui::PopStyleColor();

		ImGui::EndTable();
	}
}

void Inspector::EditTransform(float* cameraView, float* cameraProjection, float camDistance, float* local, float* world, bool& manipulated)
{
	static bool b_useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	static bool allowAxisFlip = true;


	{
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

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];

		{
			float* mat = local;
			ImGuizmo::DecomposeMatrixToComponents(mat, matrixTranslation, matrixRotation, matrixScale);
			ImGui::InputFloat3("Tr", matrixTranslation);
			ImGui::InputFloat3("Rt", matrixRotation);
			ImGui::InputFloat3("Sc", matrixScale);
			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, mat);
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


	}

	ImGui::Begin("GameWindow");
	{
		ImGui::BeginChild("Game");

		ImGuizmo::SetDrawlist();

		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		auto viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
		auto viewManipulateTop = ImGui::GetWindowPos().y;

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);

		{
			manipulated = ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, world, NULL, b_useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
		}

		// ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
		// ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

		ImGui::EndChild();
	}
	ImGui::End();
}

void Inspector::showFileDialog(StaticMeshComponent& mesh)
{
	// display
	if (ImGuiFileDialog::Instance()->Display("Load Model"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();


			int localIndex = filePathName.find("Assets");
			if (localIndex != string::npos)
			{
				filePathName = filePathName.substr(localIndex);
			}

			filePathName = StringUtil::ReplaceAll(filePathName, "\\", "/");

			mesh.Model = rm::ResourceManager::Load<rm::Model>(filePathName.c_str());

			mesh.ModelPath = filePathName;

			fileDialogOpen = false;
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}