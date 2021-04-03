#pragma once
#include <memory>
#include <stack>
#include <vector>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

#include "Camera.h"
#include "Actors/AActor.h"

class Game
{
public:
	
	Game(unsigned int framebuffer);

	void ProcessCamera();
	void ProcessInputEditor();
	void Render();

	void ProcessInput();

	void Delete(AActor* actor);

	std::stack<AActor*> KillStack;
	
	std::vector<std::unique_ptr<AActor>> Actors;

	static Camera MainCamera;

	bool Keys[1024];

	bool MouseButtons[6];

	glm::vec2 mouse;

	bool playing;

	bool mouseMoving;

private:
	unsigned int framebuffer;
	float deltaTime, lastFrame;
	
};

