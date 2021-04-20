#pragma once
#include <memory>
#include <stack>
#include <vector>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

#include "Camera.h"
#include "Actors/AActor.h"
#include "Objects/UTransformGizmo.h"

class Game
{
public:
	
	Game(unsigned int framebuffer);

	void ProcessCamera();
	void ProcessInputEditor();
	void Render();

	void ProcessInput();
	
	std::vector<std::unique_ptr<AActor>> Actors;

	std::unique_ptr<UTransformGizmo> TransformGizmo;

	static Camera MainCamera;

	bool Keys[1024];

	bool MouseButtons[6];
	bool MouseButtonsDown[6];
	bool MouseButtonsUp[6];

	glm::vec2 mouseRel;

	glm::vec2 mouse;

	glm::vec2 renderSize;

	glm::mat4 projection;

	glm::mat4 view;

	bool playing;

	bool mouseMoving;

	void DrawActorsWithPickingShader();

private:
	unsigned int framebuffer;
	float deltaTime, lastFrame;

	void Pick();
	
};

