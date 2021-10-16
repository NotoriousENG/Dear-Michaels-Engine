#pragma once
#include <memory>
#include <stack>
#include <vector>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

#include "Camera.h"
#include "Actors/AActor.h"
#include "Objects/UTransformGizmo.h"

#include "ResourceManagement/Model.h"

#include <map>
#include <ResourceManagement/Skybox.h>

namespace rm {
	class mNode;
}

class Game
{
public:

	static Game* instance;

	Game(unsigned int framebuffer);

	void ProcessCamera();

	void ProcessInputEditor();

	void Render();

	void Init();

	void ProcessInput();

	std::vector<std::shared_ptr<AActor>> Actors;

	std::unique_ptr<UTransformGizmo> TransformGizmo;

	glm::vec2 mouseRel;

	glm::vec2 mouse;

	glm::vec2 renderSize;

	glm::mat4 projection;

	glm::mat4 view;

	bool playing;

	bool mouseMoving;

	void DrawActorsWithPickingShader();

	AActor* Picked;

	bool usingPickingShader = false;

	void LoadScene(const char* path);

	void SaveScene(const char* path);

	std::unique_ptr<Skybox> skybox;

	void AddActor(std::shared_ptr<rm::Model> model);

private:

	std::shared_ptr<AActor> AddActor(rm::mNode* node);

	std::shared_ptr<rm::Shader> PickingShader;

	unsigned int framebuffer;
	float deltaTime, lastFrame;

	void Pick();

};