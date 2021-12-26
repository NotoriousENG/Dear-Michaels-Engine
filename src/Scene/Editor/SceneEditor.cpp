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

    // Iterate placeholder objects (all the same data)
    auto view = Scene::Instance->registry.view<NameComponent>();

    // use forward iterators and get only the components of interest
    for (auto entity : view)
    {
        auto &name = view.get<NameComponent>(entity);
        ShowEntity(entity, &name);
    }

    ImGui::Separator();

    // AddActor();
    ImGui::PopStyleVar();
    ImGui::End();
}

void SceneEditor::MenuItem()
{
    ImGui::MenuItem("Hierarchy", "", &isActive);
}

void SceneEditor::ShowEntity(entt::entity id, NameComponent* name)
{
    
    ImGui::PushID((int)id);
    if (name != nullptr)
    {
        ImGui::Text(name->Name.c_str());
    }

    ImGui::PopID();
}
