#pragma once
#include <glad/glad.h>

#include <imgui_impl_sdl.h>

#include <SDL2/SDL.h>

#include "Panels/IPanel.h"
#include "Panels/GameWindow.h"

#include <glm/glm.hpp>

#include <vector>

class Window
{
public:
	void execute();
	
	bool quit = false;

	SDL_GLContext maincontext;

	void ShowAppMainMenuBar();

	void ShowFileDialog();
	
private:
	
	static void sdl_die(const char* message);

	static void APIENTRY openglCallbackFunction(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	);

	void init_screen(const char* caption);

	void init_imgui();

	void processInputForWindow(SDL_Event event);
	
	void framebuffer_size_callback(int width, int height);
	
	const int SCREEN_FULLSCREEN = 0;
	int SCREEN_WIDTH = 960;
	int SCREEN_HEIGHT = 540;
	SDL_Window* window;

	GLfloat clear_color[3] = { 0.2f, 0.3f, 0.3f };

	bool show_demo_window = false;
	bool load_scene = false;
	bool save_scene = false;

	std::vector<std::unique_ptr<Panels::IPanel>> panels;

	std::unique_ptr<Panels::GameWindow> gameWindow;

	glm::vec2 prevMouse;
};


