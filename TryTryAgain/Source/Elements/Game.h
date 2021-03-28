#pragma once
#include <memory>
#include <vector>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

#include "Camera.h"
#include "Actors/AActor.h"

class Game
{
public:
	
	Game(unsigned int framebuffer);

	void Render();

	void ProcessInput();
	
	std::vector<std::unique_ptr<AActor>> Actors;

	static Camera MainCamera;

	bool Keys[1024];

	glm::vec2 mouse;

	bool focused;

	bool mouseMoving;

private:
	unsigned int framebuffer;
	float deltaTime, lastFrame;
	
};

