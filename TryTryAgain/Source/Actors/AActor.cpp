#include "AActor.h"

#include <glad/glad.h>

#include "ResourceManagement/ResourceManager.h"

AActor::AActor(const char* name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	this->name = name;
	this->transform = FTransform
	{
		pos,
		rot,
		scale
	};
}

void AActor::Draw()
{
	
}

void AActor::Tick(float delta)
{

}

void AActor::DrawPicking(int id)
{
	glm::mat4 MVP = this->projection * this->view * this->trans;

	// Convert id into unique color
	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;

	ResourceManagement::ResourceManager::GetShader("Picking").SetMatrix4("MVP", MVP);
	ResourceManagement::ResourceManager::GetShader("Picking").SetVector4f("PickingColor", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.f));

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}