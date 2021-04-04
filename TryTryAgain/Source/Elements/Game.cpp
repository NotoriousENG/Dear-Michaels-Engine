#include "Game.h"

#include "Actors/AAwesomeBox.h"
#include "Panels/Console.h"
#include "ResourceManagement/ResourceManager.h"

Camera Game::MainCamera;

Game::Game(unsigned framebuffer)
{
	Actors.push_back(std::make_unique<AAwesomeBox>("Awesome Box", glm::vec3(0, 0, 1), glm::vec3(0,45,0)));
	Actors.push_back(std::make_unique<AAwesomeBox>("Second Box", glm::vec3(1, 1, -1), glm::vec3(45, 45, 0)));

	MainCamera = Camera(glm::vec3(0,0,4), glm::vec3(0,1,0), -90);

	this->framebuffer = framebuffer;

	TransformGizmo = std::make_unique<UTransformGizmo>();
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

	if (playing)
	{
		ProcessInput();
	}
	else
	{
		ProcessInputEditor();
	}

	for (int i = 0; i < Actors.size(); i++)
	{
		if (Actors[i]->isDead)
		{
			Actors.erase(Actors.begin() + i);
			if (i >= Actors.size())
			{
				break;
			}
			i--;
		}
		else
		{
			auto* actor = Actors[i].get();
			actor->Tick(deltaTime);
			actor->Draw();
		}
		TransformGizmo->Draw();
	}
}

void Game::ProcessInput()
{
	if (this->Keys[SDLK_ESCAPE])
	{
		playing = false;
	}

	ProcessCamera();
}

void Game::ProcessInputEditor()
{
	if (this->MouseButtons[SDL_BUTTON_LEFT])
	{
		if (this->MouseButtonsDown[SDL_BUTTON_LEFT])
		{	
			if (mouse != glm::vec2(-1, -1))
			{
				// 3D Normalized device coordinates
				glm::vec2 ndcPos = (mouse / (renderSize / 2.0f) - glm::vec2(1.0f));
				ndcPos.y = -ndcPos.y;

				// Homogeneous Clip Coordinates
				glm::vec4 ray_clip = glm::vec4(ndcPos, -1, 1);

				// Camera Coordinates
				projection = glm::perspective(glm::radians(Game::MainCamera.Zoom), 1920.0f / 1080.0f, 0.1f, 100.0f);
				glm::vec4 ray_eye = inverse(projection) * ray_clip;
				ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1, 0);

				// World Coordinates
				view = glm::lookAt(Game::MainCamera.Position, Game::MainCamera.Position + Game::MainCamera.Front, Game::MainCamera.Up);
				auto ray_wor4 = (inverse(view) * ray_eye);
				glm::vec3 ray_wor = glm::vec3(ray_wor4.x, ray_wor4.y, ray_wor4.z);
				// don't forget to normalise the vector at some point
				ray_wor = glm::normalize(ray_wor);

				Actors.push_back(std::make_unique<AAwesomeBox>("Second Box", MainCamera.Position + ray_wor * 3.0f));
			}
			this->MouseButtonsDown[SDL_BUTTON_LEFT] = false;
		}
	}
	
	if (this->MouseButtons[SDL_BUTTON_RIGHT])
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_ShowCursor(0);
		ProcessCamera();
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_ShowCursor(1);
	}
}

void Game::ProcessCamera()
{
	if (mouseMoving)
	{
		MainCamera.ProcessMouseMovement(mouseRel.x, -mouseRel.y);
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
	if (this->Keys[SDLK_q])
	{
		MainCamera.ProcessKeyboard(DOWN, deltaTime);
	}
	if (this->Keys[SDLK_e])
	{
		MainCamera.ProcessKeyboard(UP, deltaTime);
	}
}
