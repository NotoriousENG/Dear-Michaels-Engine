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

void AActor::Draw()
{
	
}

void AActor::Tick(float delta)
{
	if (!isUsing)
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0), transform.position);
		glm::mat4 rotate = glm::mat4_cast(transform.rotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0), transform.scale);

		model = translate * rotate * scale;
	}
}

void AActor::DrawPicking(int id)
{
	glm::mat4 MVP = Game::MainCamera.projection * Game::MainCamera.view * this->model;

	// Convert id into unique color
	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;

	ResourceManagement::ResourceManager::GetShader("Picking").SetMatrix4("MVP", MVP);
	ResourceManagement::ResourceManager::GetShader("Picking").SetVector4f("PickingColor", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.f));

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}