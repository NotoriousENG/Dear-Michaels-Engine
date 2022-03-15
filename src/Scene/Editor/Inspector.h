#pragma once

#include <entt/entt.hpp>

#include "Modules/Editor/IEditor.h"
#include "Scene/Scene.h"

#include <ThirdParty/imgui-docking/imgui.h>
#include <ImGuiFileDialog.h>
#include <imgui_stdlib.h>
#include <ImGuizmo.h>
#include <Components.h>
#include <Utility/Utility.h>

class Inspector : public IEditor
{
public:
    virtual void Draw() override;
    virtual void MenuItem() override;

    void ShowComponents();

private:
    friend class Scene;

    void EditTransform(float *cameraView, float *cameraProjection, float camDistance, float *local, float *world, bool &manipulated);

    template<typename T>
    void EditAssetInstance(std::shared_ptr<T>& asset, std::string& serializedPath, std::string assetDescriptiveName, std::vector<std::string> fileExtensions)
    {
        static_assert(std::is_base_of<rm::Resource, T>::value, "Asset must derive from Resource");

        if (ImGui::Button(assetDescriptiveName.c_str()))
        {
            std::string extListStr = assetDescriptiveName;
            extListStr += "(";
            for (auto& ext : fileExtensions)
            {
                extListStr += "*";
                extListStr += ext;
                extListStr += ";";
            }
            extListStr += ")";

            extListStr += "{";
            for (auto& ext : fileExtensions)
            {
                extListStr += ext;
                extListStr += ",";
            }
            extListStr += "},.*";

            ImGuiFileDialog::Instance()->OpenDialog("Load Asset", FString("Load %s", assetDescriptiveName.c_str()).Text, extListStr.c_str(), "Assets/");
            fileDialogOpen = true;
        }

        float margin = 2;

        ImGui::SameLine();
        auto pos = ImGui::GetCursorScreenPos();
        ImGui::PushTextWrapPos();
        ImGui::Text(asset->path.c_str());
        ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImColor(1.0f, 1.0f, 1.0f, 0.25f));
        ImGui::PopTextWrapPos();

        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("ASSET_PATH");
            if (payload != nullptr)
            {
                std::string data = (const char*)payload->Data;

                bool found = false;
                for (auto& ext : fileExtensions)
                {
                    found |= data.find(ext) != std::string::npos;
                }

                if (found) {
                    asset = rm::ResourceManager::Load<T>(data.c_str());

                    serializedPath = data;
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (fileDialogOpen) {
            showFileDialog(asset, serializedPath);
        }
        else if (ImGuiFileDialog::Instance()->IsOpened("Load Asset"))
        {
            ImGuiFileDialog::Instance()->Close();
        }
    }

    bool useSnap = false;
    ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::LOCAL;

    bool fileDialogOpen = false;

    void ShowComponentHeader(const char *title, Entity entity, std::function<void(Entity entity)> deleteCallback);

    template <typename T>
    void showFileDialog(std::shared_ptr<T>& asset, std::string& assetPath)
    {

        static_assert(std::is_base_of<rm::Resource, T>::value, "Asset must derive from rm::Resource");

        // display
        if (ImGuiFileDialog::Instance()->Display("Load Asset"))
        {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

                int localIndex = filePathName.find("Assets");
                if (localIndex != string::npos)
                {
                    filePathName = filePathName.substr(localIndex);
                }

                filePathName = StringUtil::ReplaceAll(filePathName, "\\", "/");

                asset = rm::ResourceManager::Load<T>(filePathName.c_str());

                assetPath = filePathName;

                fileDialogOpen = false;
            }

            // close
            ImGuiFileDialog::Instance()->Close();
        }
    }

    void addComponentButton(Entity entity);
};
