#include "Game.h"

Game::Game(unsigned framebuffer)
{
	GameObjects.push_back(std::make_unique<Actor>());

	this->framebuffer = framebuffer;
}

void Game::Render()
{
	float currentFrame = SDL_GetTicks();
	deltaTime = (currentFrame - lastFrame) * 0.001f;
	lastFrame = currentFrame;

	// first pass draw scene normally
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	for (auto& GameObject : GameObjects)
	{
		GameObject->Tick(deltaTime);
		GameObject->Draw();
	}
}
