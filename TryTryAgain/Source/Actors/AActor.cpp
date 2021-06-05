#include "AActor.h"

#include <glad/glad.h>

#include <glm/gtx/matrix_decompose.hpp>

#include "Elements/Game.h"

#include "ResourceManagement/ResourceManager.h"

AActor::AActor(const char* name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	this->name = name;
	this->transform = FTransform
	{
		pos,
		glm::radians(rot),
		scale
	};
	model = glm::mat4(1);
}

void AActor::Tick(float delta)
{
	for (auto& c : components)
	{
		c->Tick(delta);
	}
	if (!isUsing)
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0), transform.position);
		glm::mat4 rotate = glm::mat4_cast(transform.rotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0), transform.scale);

		model = translate * rotate * scale;
	}
}

glm::mat4 AActor::GetMVP()
{
	return Game::MainCamera.projection * Game::MainCamera.view * this->model;
}