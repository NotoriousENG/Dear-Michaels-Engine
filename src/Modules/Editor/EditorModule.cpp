#include "EditorModule.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <SDL2/SDL.h>
#include <Modules/Editor/EditorStyles.h>

void EditorModule::Init(unsigned int texColorBuffer)
{
	this->texColorBuffer = texColorBuffer;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	Michael::StyleColorsEngine();
	// ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void EditorModule::Update()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(SDL_GL_GetCurrentWindow());
	ImGui::NewFrame();
	// ImGuizmo::BeginFrame();

	ImGui::DockSpaceOverViewport();

	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("GameWindow");
	{
		bool playing = false;
		const char* playButtonLabel = playing ? "Press Escape to Exit Play Mode" : "Play (F5)";
		if (ImGui::Button(playButtonLabel))
		{
			playing = !playing;
		}
		// Using a Child allow to fill all the space of the window.
		// It also alows customization
		ImGui::BeginChild("Game");

		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();

		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)texColorBuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::EndChild();

	}
	ImGui::End();

    se.Draw();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}

void EditorModule::Shutdown()
{
	// Cleanup imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
