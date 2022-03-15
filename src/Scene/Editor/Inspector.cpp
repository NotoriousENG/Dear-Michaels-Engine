#include "Inspector.h"
#include <Entity.h>
#include <Scene/Components.h>
#include <Elements/Camera.h>
#include <ResourceManagement/ResourceManager.h>
#include <Utility/Utility.h>

#include <ComponentRegistry.h>

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
	if (!Scene::Instance->selectedEntity)
	{
		return;
	}

	auto entity = Entity(*Scene::Instance->selectedEntity, Scene::Instance);

	if (entity.HasComponent<NameComponent>())
	{

		ShowComponentHeader("Name", entity, [](Entity entity) {
			entity.DestroyEntity(); 
		});

		if (!Scene::Instance->selectedEntity)
		{
			return;
		}

		auto& name = entity.GetComponent<NameComponent>();
		ImGui::InputText("Name", &name.Name);

	}

	if (entity.HasComponent<TransformComponent>())
	{
		ShowComponentHeader("Transform", entity, [](Entity entity) {entity.RemoveComponent<TransformComponent>(); });

		if (!entity.HasComponent<TransformComponent>())
		{
			return;
		}

		auto transform = glm::value_ptr(entity.GetComponent<TransformComponent>().transform);
		auto& camera = Camera::Main;
		bool manipulated = false;

		EditTransform(glm::value_ptr(camera.view), glm::value_ptr(camera.projection), glm::distance(glm::normalize(camera.position), glm::vec3(0)), transform, transform, manipulated);
	}

	if (entity.HasComponent<StaticMeshComponent>())
	{
		ShowComponentHeader("Mesh", entity, [](Entity entity) {entity.RemoveComponent<StaticMeshComponent>(); });

		if (!entity.HasComponent<StaticMeshComponent>())
		{
			return;
		}

		auto& mesh = entity.GetComponent<StaticMeshComponent>();

		EditAssetInstance(mesh.Model, mesh.ModelPath, "Model", mesh.Model->ValidExtensions());
	}

	if (entity.HasComponent<SpriteComponent>())
	{
		ShowComponentHeader("Sprite", entity, [](Entity entity) {entity.RemoveComponent<SpriteComponent>(); });

		if (!entity.HasComponent<SpriteComponent>())
		{
			return;
		}

		auto& sprite = entity.GetComponent<SpriteComponent>();

		EditAssetInstance(sprite.Sprite, sprite.TexturePath, "Sprite", sprite.Sprite->ValidExtensions());
	}

	if (entity.HasComponent<MovementComponent>())
	{
		ShowComponentHeader("Movement", entity, [](Entity entity) {entity.RemoveComponent<MovementComponent>(); });

		if (!entity.HasComponent<MovementComponent>())
		{
			return;
		}

		auto& move = entity.GetComponent<MovementComponent>();

		ImGui::InputFloat("Speed", &move.speed);
	}


	addComponentButton(entity);

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

void Inspector::ShowComponentHeader(const char* title, Entity entity, std::function<void(Entity entity)> deleteCallback)
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
			deleteCallback(entity);
			return;
		}
		ImGui::PopStyleColor();

		ImGui::EndTable();
	}
}

void Inspector::addComponentButton(Entity entity)
{
	ImGui::Separator();

	// Simple selection popup (if you want to show the current selection inside the Button itself,
	// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("Add Component Popup");
	if (ImGui::BeginPopup("Add Component Popup"))
	{
		ImGui::Text("Components: ");
		ImGui::Separator();
		for (auto& c : ComponentRegistry::registeredComponents)
		{
			if (ImGui::Selectable(c.first.c_str()))
			{
				if (auto type = entt::resolve(c.second))
				{
					if (auto func = type.func(entt::hashed_string("Assign")))
					{
						func.invoke({}, entity);
					}
				}
			}
		}
		ImGui::EndPopup();
	}
}
