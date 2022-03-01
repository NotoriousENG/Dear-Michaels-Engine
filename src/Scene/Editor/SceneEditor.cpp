#include "SceneEditor.h"

#include <string.h>

#include <Scene/Components.h>

#include <imgui.h>

void SceneEditor::Draw()
{
    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    ImGui::Begin("Hierarchy");

    ImGui::PushStyleColor(0, ImVec4(.5f, 0, 0, 1));
    ImGui::Text("Remove Parent");
    ImGui::PopStyleColor();

    ImGui::Separator();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

    auto view = Scene::Instance->registry.view<NameComponent>();

    // use forward iterators and get only the components of interest
    for (auto entity : view)
    {
        auto &name = view.get<NameComponent>(entity);
        ShowEntity(entity, &name);
    }

    ImGui::Separator();

    AddEntity();

    ImGui::PopStyleVar();
    ImGui::End();
}

void SceneEditor::MenuItem()
{
    ImGui::MenuItem("Hierarchy", "", &bDrawable);
}

void SceneEditor::ShowEntity(entt::entity id, NameComponent* name)
{
    
    ImGui::PushID((int)id);
    if (name != nullptr)
    {
        if (Scene::Instance->selectedEntity != nullptr && id == *Scene::Instance->selectedEntity)
        {
            ImGui::PushStyleColor(0, ImVec4(0, 1, 1, 1));
        }

        ImGui::Text(name->Name.c_str());

        if (Scene::Instance->selectedEntity != nullptr && id == *Scene::Instance->selectedEntity)
        {
            ImGui::PopStyleColor();
        }

        // Pick actor from label
        if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), true) && ImGui::IsMouseClicked(0))
        {
            Scene::Instance->selectedEntity = new entt::entity(id);
        }
    }

    ImGui::PopID();
}

void SceneEditor::AddEntity()
{
    if (ImGui::Button("Add Entity"))
    {
        Scene::Instance->CreateEntity();
    }
}
