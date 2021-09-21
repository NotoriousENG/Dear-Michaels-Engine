#include "Inspector.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Elements/Game.h"
#include <glad/glad.h>
#include "Panels/Console.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <Panels/GameWindow.h>
#include "Components/UStaticModelComponent.h"
#include "imgui_stdlib.h"
#include "Utility/Utility.h"
#include <vector>
#include <string>
#include <Input/Input.h>
#include "Actors/AActor.h"

namespace Panels
{
	static int regIndex = 0;

	UComponent* Inspector::InspectedComponent = nullptr;

	static ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::LOCAL;

	void Inspector::Draw()
	{
		auto& camera = Camera::Main;

		if (!Input::MouseButtons[SDL_BUTTON_RIGHT])
		{
			if (Input::Keys[SDLK_w])
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (Input::Keys[SDLK_e])
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			if (Input::Keys[SDLK_r]) // r Key
				mCurrentGizmoOperation = ImGuizmo::SCALE;
			if (Input::Keys[SDLK_t])
				mCurrentGizmoOperation = ImGuizmo::BOUNDS;
		}

		ImGui::Begin("Inspector");

		if (!Game::instance->Actors.empty() && Game::instance->Picked != nullptr)
		{
			auto* actor = Game::instance->Picked;

			glm::mat4 mat = mCurrentGizmoMode == ImGuizmo::MODE::LOCAL ? Game::instance->Picked->transform->GetModelMatrixLocal() : Game::instance->Picked->transform->GetModelMatrixWorld();

			float* model_arr = glm::value_ptr(mat);
			/*float* l_model_arr = glm::value_ptr(local_mat);*/

			ImGuizmo::SetGizmoSizeClipSpace(0.2);

			auto mPrevGizmoMode = mCurrentGizmoMode;

			EditTransform(glm::value_ptr(camera.view), glm::value_ptr(camera.projection), glm::distance(glm::normalize(camera.position), glm::vec3(0)), model_arr, nullptr);

			if (actor != nullptr)
			{
				if (mPrevGizmoMode == mCurrentGizmoMode)
				{
					glm::vec3 scale;
					glm::quat rot;
					glm::vec3 pos;
					glm::vec3 skew;
					glm::vec4 persp;

					if (mCurrentGizmoMode == ImGuizmo::MODE::LOCAL)
					{
						glm::decompose(mat, scale, rot, pos, skew, persp);
						actor->transform->SetLocalPosition(pos);
						actor->transform->SetLocalRotation(rot);
						actor->transform->SetLocalScale(scale);
					}
					else
					{
						glm::decompose(mat, scale, rot, pos, skew, persp);
						actor->transform->SetPosition(pos);
						actor->transform->SetRotation(rot);
						actor->transform->SetScale(scale);
					}
				}
				


				int i = 0;
				for (auto& comp : actor->components)
				{
					ImGui::PushID(i);
					if (!comp->ShowInspector())
					{
						ImGui::PopID();
						ImGui::End();
						return;
					}
					ImGui::PopID();
					i++;
				}

				addComponentField();
			}
			else {
				InspectedComponent = nullptr;
			}
		}

		ImGui::End();

	}

	void Inspector::MenuItem()
	{
		ImGui::MenuItem("Inspector", "", &isActive);
	}

	void Inspector::EditTransform(float* cameraView, float* cameraProjection, float camDistance, float* world_mat, float* local_mat)
	{
		static bool b_useSnap = false;
		static float snap[3] = { 1.f, 1.f, 1.f };
		static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
		static bool boundSizing = false;
		static bool boundSizingSnap = false;

		static bool allowAxisFlip = true;


		{
			auto actor = Game::instance->Picked;
			ImGui::BeginTable("split", 3, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2);
			ImGui::InputText("", &actor->name);
			ImGui::PopItemWidth();

			ImGui::TableSetColumnIndex(1);
			int uid = reinterpret_cast<unsigned int>(actor);
			ImGui::Text("ID: 0x%x", uid);
			ImGui::TableSetColumnIndex(2);

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 1));

			if (ImGui::Button("X"))
			{
				Game::instance->Picked = nullptr;
				Destroy(actor);
				ImGui::PopStyleColor();
				ImGui::EndTable();

				return;
			}
			ImGui::PopStyleColor();

			ImGui::EndTable();

			ImGui::Text("____________________________________________________________________");
			ImGui::Text("Transform:");

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
				float* mat = /*(mCurrentGizmoMode == ImGuizmo::LOCAL) ? local_mat :*/ world_mat;
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

			if (!Game::instance->playing)
			{
				ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, world_mat, NULL, b_useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
			}

			// ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
			// ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

			if (false)
			{
				//ImGui::End();
				ImGui::PopStyleColor(1);
			}

			ImGui::EndChild();
		}
		ImGui::End();
	}

	void Inspector::addComponentField()
	{
		// Add Component
		ImGui::Text("____________________________________________________________________");
		ImGui::NewLine();
		if (ImGui::Button("AddComponent"))
		{
			auto comp = ComponentFactory::instance.Create(ComponentFactory::instance.Keys[regIndex]);
			comp->Init();
			Game::instance->Picked->AddComponent(std::move(comp));
		}
		ImGui::SameLine();
		ImGui::Combo("Component", &regIndex, &ComponentFactory::instance.Keys[0], ComponentFactory::instance.Keys.size(), 5);
	}
}
