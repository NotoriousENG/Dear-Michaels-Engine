#include "Game.h"

#include "Actors/AAwesomeBox.h"

Camera Game::MainCamera;

Game::Game(unsigned framebuffer)
{
	Actors.push_back(std::make_unique<AAwesomeBox>("Awesome Box", glm::vec3(0, 0, 1), glm::vec3(0,45,0)));
	Actors.push_back(std::make_unique<AAwesomeBox>("Second Box", glm::vec3(1, 1, -1), glm::vec3(45, 45, 0)));

	MainCamera = Camera(glm::vec3(0,0,4), glm::vec3(0,1,0), -90);

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
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now

	if (focused)
	{
		ProcessInput();
	}

	for (auto& actor : Actors)
	{
		actor->Tick(deltaTime);
		actor->Draw();
	}
}

void Game::ProcessInput()
{
	if (mouseMoving)
	{
		MainCamera.ProcessMouseMovement(mouse.x, -mouse.y);
		mouseMoving = false;
	}
	
	
	if (this->Keys[SDLK_w])
	{
		MainCamera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (this->Keys[SDLK_s])
	{
		MainCamera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (this->Keys[SDLK_a])
	{
		MainCamera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (this->Keys[SDLK_d])
	{
		MainCamera.ProcessKeyboard(RIGHT, deltaTime);
	}

}
