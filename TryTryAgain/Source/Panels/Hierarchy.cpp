#include "Hierarchy.h"


#include "Actors/AAwesomeBox.h"
#include "Elements/Game.h"
#include "Structs/FString.h"

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
        unsigned long uid = reinterpret_cast<unsigned long>(actor);
        ImGui::PushID(uid);

        // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        bool node_open = ImGui::TreeNode("Object", "%s", actor->name);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("ID: %lu", uid);

        if (node_open)
        {
            {
                ImGui::PushID(0); // Use field index as identifier.
                ShowTransform(actor->transform);

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

            {
                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                ImGui::TreeNodeEx("Rotation", flags, "Rotation: ");

                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputFloat3("Rotation", &transform.rotation.x);

                ImGui::NextColumn();
            }

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
		}
        
	}

	void Hierarchy::AddActor()
	{
        if (ImGui::Button("Add Actor"))
        {
            int id = MyGame->Actors.size();
            MyGame->Actors.push_back(std::make_unique<AAwesomeBox>(FString("AwesomeBox (%i)", id).Text));
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
        }
	}
}