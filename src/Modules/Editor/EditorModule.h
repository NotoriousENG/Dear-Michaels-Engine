#pragma once

#include <vector>
#include <memory>
#include <Modules/Editor/IEditor.h>
#include <functional>
#include <imgui_impl_sdl.h>
#include <GL_RenderModule.h>

class EditorModule
{
public:
	void Init(GL_RenderModule* renderModule);
	void Update();
	void Shutdown();

    std::vector<std::unique_ptr<IEditor>> editors;

	static bool playing;
    bool show_demo_window = false;

private:
	unsigned int texColorBuffer;
	ImVec2 size = ImVec2(1920, 1080);

	GL_RenderModule* renderModule;
};

