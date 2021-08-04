#include "Hierarchy.h"

#include <string>

#include <glm/gtc/type_ptr.hpp>

#include "Console.h"
#include "Actors/AAwesomeBox.h"
#include "Elements/Game.h"
#include "Structs/FString.h"
#include "imgui_stdlib.h"

namespace Panels
{
	Hierarchy::Hierarchy(Game* game)
	{
		MyGame = game;
	}

	void Hierarchy::HelpIcon(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void Hierarchy::ShowActor(AActor* actor)
	{
        int uid = reinterpret_cast<unsigned int>(actor);
        ImGui::PushID(uid);

        // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        const char* actorLabel = actor->name.c_str();

        auto wasEditing = actor->isEditing;

        if (actor == MyGame->Picked)
            ImGui::PushStyleColor(0, ImVec4(0, 1, 1, 1));

        actor->isEditing = ImGui::TreeNode("Object", actorLabel);

        if (actor == MyGame->Picked)
            ImGui::PopStyleColor();

        if (actor->isEditing != wasEditing)
        {
            MyGame->Picked = actor;
        }

        ImGui::TableSetColumnIndex(1);

        if (actor == MyGame->Picked)
            ImGui::PushStyleColor(0, ImVec4(0, 1, 1, 1));

        ImGui::Text("ID: 0x%x", uid);
        
        if (actor == MyGame->Picked)
            ImGui::PopStyleColor();

        if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), true) && ImGui::IsMouseClicked(0))
        {
            MyGame->Picked = actor;
        }

        

        if (actor->isEditing)
        {
            {
                ImGui::TableSetColumnIndex(0);

                if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), true) && ImGui::IsMouseClicked(0))
                {
                    MyGame->Picked = actor;
                }

                ImGui::TableSetColumnIndex(1);
            	
                ImGui::PushID("Transform"); // Use field index as identifier.
                // ShowTransform(actor->transform);
                ImGui::PopID();
            	
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
	}

	void Hierarchy::ShowTransform(FTransform &transform)
	{
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        bool transform_open = ImGui::TreeNode("Transform", "Transform");

		if (transform_open)
		{
            ImGui::PushID("Position");
            {
                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                ImGui::TreeNodeEx("Position", flags, "Position: ");

                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputFloat3("Position", &transform.position.x);

                ImGui::NextColumn();
            }
            ImGui::PopID();

            ImGui::PushID("Rotation");
            {
                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                ImGui::TreeNodeEx("Rotation", flags, "Rotation: ");

                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);

                glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(transform.rotation));
                if (ImGui::InputFloat3("Rotation", &eulerAngles[0])) {
                    transform.rotation = glm::quat(glm::radians(eulerAngles));
                }

                ImGui::NextColumn();
            }
            ImGui::PopID();

            ImGui::PushID("Scale");
            {
                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                ImGui::TreeNodeEx("Scale", flags, "Scale: ");

                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputFloat3("Scale", &transform.scale.x);

                ImGui::NextColumn();
            }
            ImGui::PopID();

            ImGui::TreePop();
		}
        
	}

	void Hierarchy::AddActor()
	{
        if (ImGui::Button("Add Actor"))
        {
            int id = MyGame->Actors.size();
            MyGame->Actors.push_back(std::make_unique<AAwesomeBox>());
            MyGame->Picked = MyGame->Actors.back().get();
            MyGame->Picked->name = FString("AwesomeBox (%i)", id).Text;
            MyGame->Picked->UpdateMatrix();
        }
	}

	void Hierarchy::Draw(bool* p_open)
	{
        if (*p_open)
        {
            ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
            if (!ImGui::Begin("Hierarchy", p_open))
            {
                ImGui::End();
                return;
            }

            HelpIcon(
                "This is a simple property editor");

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
            if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
            {
                // Iterate placeholder objects (all the same data)
                for (auto& actor : MyGame->Actors)
                {
                    ShowActor(actor.get());
                    //ImGui::Separator();
                }
                ImGui::EndTable();
            }
            AddActor();
            ImGui::PopStyleVar();
            ImGui::End();

            ImGui::Begin("Inspector");
            ImGui::End();
        }
	}
}
