#include "Hierarchy.h"

#include <string>

#include <glm/gtc/type_ptr.hpp>

#include "Console.h"
#include "Elements/Game.h"
#include "Structs/FString.h"
#include "imgui_stdlib.h"
#include <Components/UStaticModelComponent.h>

namespace Panels
{

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

        if (actor == Game::instance->Picked)
            ImGui::PushStyleColor(0, ImVec4(0, 1, 1, 1));

        actor->isEditing = ImGui::TreeNode("Object", actorLabel);

        if (actor == Game::instance->Picked)
            ImGui::PopStyleColor();

        if (actor->isEditing != wasEditing)
        {
            Game::instance->Picked = actor;
        }

        ImGui::TableSetColumnIndex(1);

        if (actor == Game::instance->Picked)
            ImGui::PushStyleColor(0, ImVec4(0, 1, 1, 1));

        ImGui::Text("ID: 0x%x", uid);
        
        if (actor == Game::instance->Picked)
            ImGui::PopStyleColor();

        if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), true) && ImGui::IsMouseClicked(0))
        {
            Game::instance->Picked = actor;
        }

        

        if (actor->isEditing)
        {
            {
                ImGui::TableSetColumnIndex(0);

                if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), true) && ImGui::IsMouseClicked(0))
                {
                    Game::instance->Picked = actor;
                }

                ImGui::TableSetColumnIndex(1);
            	
            	
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
	}

	void Hierarchy::AddActor()
	{
        ImGui::Text("Add Actor:");
        if (ImGui::Button("Empty"))
        {
            int id = Game::instance->Actors.size();
            Game::instance->Actors.push_back(std::make_shared<AActor>());
            Game::instance->Picked = Game::instance->Actors.back().get();
            Game::instance->Picked->name = FString("Actor (%i)", id).Text;
            Game::instance->Picked->UpdateMatrix();
        }
        if (ImGui::Button("3D Model"))
        {
            int id = Game::instance->Actors.size();
            Game::instance->Actors.push_back(std::make_shared<AActor>());
            Game::instance->Picked = Game::instance->Actors.back().get();
            Game::instance->Picked->name = FString("Actor (%i)", id).Text;
            Game::instance->Picked->UpdateMatrix();

            auto& a = Game::instance->Actors.back();
            a->AddComponent<UStaticModelComponent>();
            a->Init();
        }
	}

	void Hierarchy::Draw()
	{
        ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Hierarchy"))
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
            for (auto& actor : Game::instance->Actors)
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
    void Hierarchy::MenuItem()
    {
        ImGui::MenuItem("Hierarchy", "", &isActive);
    }
}
