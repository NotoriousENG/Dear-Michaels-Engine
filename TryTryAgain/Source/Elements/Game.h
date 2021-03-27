#pragma once
#include <memory>
#include <vector>
#include <SDL2/SDL_timer.h>


#include "Actor.h"

class Game
{
public:
	
	Game(unsigned int framebuffer);

	void Render();
	
	std::vector<std::unique_ptr<Actor>> GameObjects;

private:
	unsigned int framebuffer;
	float deltaTime, lastFrame;
};

