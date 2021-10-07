#include "Game.h"

#include "Panels/Console.h"
#include "ResourceManagement/ResourceManager.h"
#include <Components/UStaticModelComponent.h>
#include <Structs/FString.h>

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>

#include <iostream>
#include <fstream>
#include "Input/Input.h"
#include <Components/UStaticMeshComponent.h>

Game* Game::instance;

Game::Game(unsigned framebuffer)
{
	Game::instance = this;

	Camera::Main = Camera(glm::vec3(0, 0, 4), glm::vec3(0, 1, 0), -90);

	this->framebuffer = framebuffer;

	TransformGizmo = std::make_unique<UTransformGizmo>();

	PickingShader = rm::ResourceManager::Load<rm::Shader>("Assets/Shaders/Picking.glsl");

	Init();
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
				auto model_comp = actor->GetComponent<UStaticModelComponent>();
				if (model_comp != nullptr && model_comp->isActive)
					model_comp->Draw();

				auto mesh_comp = actor->GetComponent<UStaticMeshComponent>();
				if (mesh_comp != nullptr && mesh_comp->isActive)
					mesh_comp->Draw();

			}
		}
	}

	skybox->Render();
}

void Game::Init()
{

	for (auto& actor : Actors)
	{
		actor->Init();
	}

	skybox = std::make_unique<Skybox>();
}

void Game::ProcessInput()
{
	if (Input::Keys[SDLK_ESCAPE])
	{
		playing = false;
	}
}

void Game::DrawActorsWithPickingShader()
{
	// Picking

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	PickingShader->Use();

	// Only positions are needed
	glEnableVertexAttribArray(0);


	int i = 0;
	for (auto& a : Actors)
	{
		auto model_comp = a->GetComponent<UStaticModelComponent>();
		if (model_comp != nullptr)
		{
			// Convert id into unique color
			int r = (i & 0x000000FF) >> 0;
			int g = (i & 0x0000FF00) >> 8;
			int b = (i & 0x00FF0000) >> 16;

			PickingShader->SetMatrix4("MVP", a->GetMVP());
			PickingShader->SetVector4f("PickingColor", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.f));

			model_comp->Model->Draw(PickingShader.get());
		}

		auto mesh_comp = a->GetComponent<UStaticMeshComponent>();
		if (mesh_comp != nullptr)
		{
			// Convert id into unique color
			int r = (i & 0x000000FF) >> 0;
			int g = (i & 0x0000FF00) >> 8;
			int b = (i & 0x00FF0000) >> 16;

			PickingShader->SetMatrix4("MVP", a->GetMVP());
			PickingShader->SetVector4f("PickingColor", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.f));

			mesh_comp->Mesh->Draw(PickingShader.get());
		}

		i++;
	}
}

void Game::LoadScene(const char* path)
{
	std::ifstream is(path);
	cereal::JSONInputArchive iarchive(is);
	Picked = nullptr;
	Actors.clear();
	iarchive(Actors);
	for (auto& a : Actors)
	{
		a->Init();
	}
}

void Game::SaveScene(const char* path)
{
	// save actors
	std::ofstream os(path);
	cereal::JSONOutputArchive oarchive(os);

	oarchive(CEREAL_NVP(Actors));
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
	if (Input::Keys[SDLK_1])
	{
		DrawActorsWithPickingShader();
		usingPickingShader = true;
	}
	else
	{
		usingPickingShader = false;
	}

	if (Input::Keys[SDLK_DELETE] && this->Picked != nullptr)
	{
		auto toDestroy = Picked;
		Picked = nullptr;
		Destroy(toDestroy);
	}

	if (Input::MouseButtonsUp[SDL_BUTTON_LEFT] && ImGui::IsWindowFocused())
	{
		if (mouse != glm::vec2(-1, -1))
		{
			Pick();
		}
		usingPickingShader = true;
	}

	if (Input::MouseButtons[SDL_BUTTON_RIGHT])
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
		Camera::Main.ProcessMouseMovement(mouseRel.x, -mouseRel.y);
		mouseMoving = false;
	}


	if (Input::Keys[SDLK_w])
	{
		Camera::Main.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	if (Input::Keys[SDLK_s])
	{
		Camera::Main.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (Input::Keys[SDLK_a])
	{
		Camera::Main.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	if (Input::Keys[SDLK_d])
	{
		Camera::Main.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
	if (Input::Keys[SDLK_q])
	{
		Camera::Main.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
	}
	if (Input::Keys[SDLK_e])
	{
		Camera::Main.ProcessKeyboard(Camera_Movement::UP, deltaTime);
	}
}
