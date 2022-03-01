#pragma once 

#include <entt/entt.hpp>

#include "Modules/Editor/IEditor.h"
#include "Scene/Scene.h"

#include <ThirdParty/imgui-docking/imgui.h>
#include <imgui_stdlib.h>
#include <ImGuizmo.h>
#include <Components.h>

class Inspector : public IEditor
{
public:
    virtual void Draw() override;
    virtual void MenuItem() override;

    void ShowComponents();

private:
    friend class Scene;

    void EditTransform(float* cameraView, float* cameraProjection, float camDistance, float* local, float* world, bool& manipulated);

    bool useSnap = false;
    ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::LOCAL;

    bool fileDialogOpen = false;

    void ShowComponentHeader(const char* title, Entity entity, std::function<void(Entity entity)> deleteCallback);

    void showFileDialog(StaticMeshComponent& mesh);

    void addComponentButton(Entity entity);

};
