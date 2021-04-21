#include "AAwesomeBox.h"

#include <glad/glad.h>


#include "Elements/Game.h"
#include "ResourceManagement/ResourceManager.h"

AAwesomeBox::AAwesomeBox(const char* name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	this->name = name;

	model = glm::mat4(1);

	view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

	this->transform = FTransform
	{
		pos,
		rot,
		scale
	};

	ResourceManagement::ResourceManager::LoadShader("Assets/Shaders/Standard.vert", "Assets/Shaders/Standard.frag", nullptr, "Standard");

	ResourceManagement::ResourceManager::LoadTexture("Assets/Textures/container.jpg", false, "container");
	ResourceManagement::ResourceManager::LoadTexture("Assets/Textures/awesomeface.png", true, "awesomeface");

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
		ResourceManagement::ResourceManager::GetShader("Standard").Use();

		glActiveTexture(GL_TEXTURE0);
		ResourceManagement::ResourceManager::GetTexture("container").Bind();
		glActiveTexture(GL_TEXTURE1);
		ResourceManagement::ResourceManager::GetTexture("awesomeface").Bind();

		ResourceManagement::ResourceManager::GetShader("Standard").SetInteger("texture1", 0);
		ResourceManagement::ResourceManager::GetShader("Standard").SetInteger("texture2", 1);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}

}

void AAwesomeBox::Tick(float delta)
{
	/*glm::mat4 trans = glm::translate(model, transform.position);
	
	trans = glm::rotate (trans, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));

	trans = glm::scale(trans, transform.scale);

	model = trans;*/

	view = glm::lookAt(Game::MainCamera.Position, Game::MainCamera.Position + Game::MainCamera.Front, Game::MainCamera.Up);
	projection = glm::perspective(glm::radians(Game::MainCamera.Zoom), 1920.0f / 1080.0f, 0.1f, 100.0f);

	ResourceManagement::ResourceManager::GetShader("Standard").Use();
	ResourceManagement::ResourceManager::GetShader("Standard").SetMatrix4("model", model);
	ResourceManagement::ResourceManager::GetShader("Standard").SetMatrix4("view", view);
	ResourceManagement::ResourceManager::GetShader("Standard").SetMatrix4("projection", projection);
}
