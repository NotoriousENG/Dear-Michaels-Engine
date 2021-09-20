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

	void Hierarchy::ShowActor(std::shared_ptr<AActor> actor)
	{
        // index in actor vector
        auto it = std::find(Game::instance->Actors.begin(), Game::instance->Actors.end(), actor);
        int id = (it != Game::instance->Actors.end()) ? it - Game::instance->Actors.begin() : -1;

        ImGui::PushID(id);

        ImGui::AlignTextToFramePadding();
        const char* actorLabel = actor->name.c_str();

        actor->isExpandedInHierarchy = ImGui::TreeNode("Object", " ");

        if (actor.get() == Game::instance->Picked)
            ImGui::PushStyleColor(0, ImVec4(0, 1, 1, 1));

        ImGui::SameLine();
        ImGui::Text(actorLabel);

        if (actor.get() == Game::instance->Picked)
            ImGui::PopStyleColor();

        // Pick actor from label
        if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), true) && ImGui::IsMouseClicked(0))
        {
            Game::instance->Picked = actor.get();
        }

        // Set actor to be a child of another actor
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::BeginTooltip();
            ImGui::SetTooltip(actorLabel);
            ImGui::EndTooltip();

            // get index in actor vector
            std::string data = FString("%i", id).Text;
            ImGui::SetDragDropPayload("ACTOR", data.c_str(), data.length() + 1);

            ImGui::EndDragDropSource();
        }

        // Add child to this actor
        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("ACTOR");
            if (payload != nullptr)
            {
                std::string data = (const char*)payload->Data;
                if (data != FString("%i", id).Text) {
                    auto& child = Game::instance->Actors[std::stoi(data)];
                    FTransform::AddChild(actor->transform, child->transform);
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (actor->isExpandedInHierarchy)
        {
            for (auto& c : actor->transform->children)
            {
                if (c->actor != nullptr)
                    ShowActor(c->actor);
            }
            ImGui::TreePop();
        }

        
        ImGui::PopID();
	}

	void Hierarchy::AddActor()
	{
        ImGui::Text("Add:");
        if (ImGui::Button("Empty"))
        {
            int id = Game::instance->Actors.size();
            Game::instance->Actors.push_back(std::make_shared<AActor>());
            
            std::shared_ptr<AActor> sp = Game::instance->Actors.back();
            sp->transform->actor = sp;

            Game::instance->Picked = Game::instance->Actors.back().get();
            Game::instance->Picked->name = FString("Empty (%i)", id).Text;
        }
        if (ImGui::Button("Cube"))
        {
            int id = Game::instance->Actors.size();
            Game::instance->Actors.push_back(std::make_shared<AActor>());

            Game::instance->Picked = Game::instance->Actors.back().get();
            Game::instance->Picked->name = FString("Cube (%i)", id).Text;

            auto& a = Game::instance->Actors.back();
            a->AddComponent<UStaticModelComponent>();
            a->Init();

            std::shared_ptr<AActor> sp = Game::instance->Actors.back();
            sp->transform->actor = sp;

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

        ImGui::PushStyleColor(0, ImVec4(.5f, 0, 0, 1));
        ImGui::Text("Remove Parent");
        ImGui::PopStyleColor();
        

        // Add child to this actor
        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("ACTOR");
            if (payload != nullptr)
            {
                std::string data = (const char*)payload->Data;
                
                auto& child = Game::instance->Actors[std::stoi(data)];
                FTransform::AddChild(nullptr, child->transform);

            }
            ImGui::EndDragDropTarget();
        }

        ImGui::Separator();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

        // Iterate placeholder objects (all the same data)
        for (auto& actor : Game::instance->Actors)
        {
            if (actor->transform->parent == nullptr)
                ShowActor(actor);
        }

        ImGui::Separator();

        AddActor();
        ImGui::PopStyleVar();
        ImGui::End();
	}
    void Hierarchy::MenuItem()
    {
        ImGui::MenuItem("Hierarchy", "", &isActive);
    }
}
