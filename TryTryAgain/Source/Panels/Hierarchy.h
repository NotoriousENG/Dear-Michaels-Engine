#pragma once
#include <imgui.h>

namespace Panels
{
    class Hierarchy
    {

    public:

        Hierarchy(Game* game)
        {
            MyGame = game;
        }

        static void HelpIcon(const char* desc)
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

    	void ShowObject(Actor* actor)
        {
            int uid = reinterpret_cast<int>(actor);
            ImGui::PushID(uid);

            // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            bool node_open = ImGui::TreeNode("Object", "%s_%u", "MyActor", uid);
            ImGui::TableSetColumnIndex(1);
            // ImGui::Text("my sailor is rich");

            if (node_open)
            {
                static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
                {
                    ImGui::PushID(0); // Use field index as identifier.

                    {
                        // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                        ImGui::TreeNodeEx("Position", flags, "Position: ");

                        ImGui::TableSetColumnIndex(1);
                        ImGui::SetNextItemWidth(-FLT_MIN);
                        ImGui::InputFloat3("Position", actor->position);

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
                        ImGui::InputFloat3("Rotation", actor->rotation);

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
                        ImGui::InputFloat3("Scale", actor->scale);

                        ImGui::NextColumn();
                    }
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        void Draw(bool* p_open)
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
                    
                    {
                        ShowObject(MyGame->GameObjects[0].get());
                        //ImGui::Separator();
                    }
                    ImGui::EndTable();
                }
                ImGui::PopStyleVar();
                ImGui::End();
            }
        }

    private:

        Game* MyGame;
    	
    };
}