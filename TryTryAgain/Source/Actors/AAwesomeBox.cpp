#include "AAwesomeBox.h"

#include <glad/glad.h>


#include "Elements/Game.h"
#include "ResourceManagement/ResourceManager.h"

AAwesomeBox::AAwesomeBox(const char* name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : AActor(name, pos, rot, scale)
{

	rm::ResourceManager::LoadShader("Assets/Shaders/Standard.vert", "Assets/Shaders/Standard.frag", nullptr, "Standard");

	rm::ResourceManager::LoadTexture("Assets/Textures/container.jpg", false, "container");
	rm::ResourceManager::LoadTexture("Assets/Textures/awesomeface.png", true, "awesomeface");

	// -------------------------------------VAO/VBO------------------------------------------------------

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int t = sizeof(vertices) / sizeof(vertices[0]);
	numVertices = t / 5;

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void AAwesomeBox::Draw()
{
	if (toDraw)
	{
		rm::ResourceManager::GetShader("Standard").Use();

		glActiveTexture(GL_TEXTURE0);
		rm::ResourceManager::GetTexture("container").Bind();
		glActiveTexture(GL_TEXTURE1);
		rm::ResourceManager::GetTexture("awesomeface").Bind();

		rm::ResourceManager::GetShader("Standard").SetInteger("texture1", 0);
		rm::ResourceManager::GetShader("Standard").SetInteger("texture2", 1);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}

}

void AAwesomeBox::Tick(float delta)
{
	Super::Tick(delta);

	rm::ResourceManager::GetShader("Standard").Use();
	rm::ResourceManager::GetShader("Standard").SetMatrix4("model", model);
	rm::ResourceManager::GetShader("Standard").SetMatrix4("view", Game::MainCamera.view);
	rm::ResourceManager::GetShader("Standard").SetMatrix4("projection", Game::MainCamera.projection);
}
