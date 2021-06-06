#include "Game.h"

#include "Actors/AAwesomeBox.h"
#include "Panels/Console.h"
#include "ResourceManagement/ResourceManager.h"
#include <Components/UStaticMeshComponent.h>

Camera Game::MainCamera;

Game::Game(unsigned framebuffer)
{
	rm::ResourceManager::LoadShader("Assets/Shaders/Lit.vert", "Assets/Shaders/Lit.frag", nullptr, "Lit");
	rm::ResourceManager::LoadShader("Assets/Shaders/Picking.vert", "Assets/Shaders/Picking.frag", nullptr, "Picking");

	Actors.push_back(std::make_unique<AAwesomeBox>("Awesome Box", glm::vec3(0, 0, 1), glm::vec3(0,45,0)));

	Actors.push_back(std::make_unique<AAwesomeBox>("Second Box", glm::vec3(1, 1, -1), glm::vec3(45, 45, 0)));

	MainCamera = Camera(glm::vec3(0,0,4), glm::vec3(0,1,0), -90);

	this->framebuffer = framebuffer;

	TransformGizmo = std::make_unique<UTransformGizmo>();

	char buf[128];

	for (int i = 0; i < 1000; i++)
	{
		float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 100) - 50;
		float y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 100) - 50;
		float z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 100) - 50;

		float rx = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 100) - 50;
		float ry = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 100) - 50;
		float rz = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 100) - 50;
		
		sprintf(buf, "Box [%i]", i);
		Actors.push_back(std::make_unique<AAwesomeBox>(buf, glm::vec3(x, y, z), glm::vec3(rx, ry, rz)));
	}
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

	TransformGizmo->Draw();

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
			if (playing)
				actor->Tick(deltaTime);
			if (!usingPickingShader)
			{
				auto mesh_comp = actor->GetComponent<UStaticMeshComponent>();
				if (mesh_comp != nullptr)
					mesh_comp->Draw();
			}
		}
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

void Game::DrawActorsWithPickingShader()
{
	// Picking
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	rm::ResourceManager::GetShader("Picking")->Use();

	// Only positions are needed
	glEnableVertexAttribArray(0);


	int i = 0;
	for (auto& a : Actors)
	{
		auto mesh_comp = a->GetComponent<UStaticMeshComponent>();
		if (mesh_comp != nullptr)
			mesh_comp->Mesh->DrawPicking(i, a->GetMVP());
		i++;
	}
}

void Game::Pick()
{
	float h = ImGui::GetWindowHeight();
	float w = ImGui::GetWindowWidth();

	// 3D Normalized device coordinates
	glm::vec2 ndcPos = (mouse / (renderSize / 2.0f) - glm::vec2(1.0f));
	ndcPos.y = -ndcPos.y;

	// Screen coords Framebuffer agnostic
	ndcPos += glm::vec2(1);
	ndcPos /= 2;
	ndcPos *= glm::vec2(w, h);
	
	DrawActorsWithPickingShader();

	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	glReadPixels(ndcPos.x, ndcPos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	int id = data[0] + data[1] * 256 + data[2] * 256 * 256;
	/*M_LOG("%x", id);
	M_LOG("(%f, %f)", ndcPos.x, ndcPos.y);*/
	if (id < Actors.size())
	{
		Picked = Actors.at(id).get();
		// M_LOG("Picked: %s ID: 0x%x", Picked->name.c_str(), reinterpret_cast<int>(Picked));
	}
	else
	{
		Picked = nullptr;
	}
	
}

void Game::ProcessInputEditor()
{
	if (this->Keys[SDLK_1])
	{
		DrawActorsWithPickingShader();
		usingPickingShader = true;
	}
	else
	{
		usingPickingShader = false;
	}

	if (this->Keys[SDLK_DELETE] && this->Picked != nullptr)
	{
		auto toDestroy = Picked;
		Picked = nullptr;
		Destroy(toDestroy);
	}
	
	if (this->MouseButtonsUp[SDL_BUTTON_LEFT])
	{
		if (mouse != glm::vec2(-1, -1))
		{
			Pick();
		}
	}

	if (this->MouseButtonsUp[SDL_BUTTON_LEFT])
	{
		usingPickingShader = true;
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
