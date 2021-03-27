#include "Actor.h"

Actor::Actor()
{
	model = glm::mat4(1);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	ResourceManager::LoadShader("Assets/Shaders/Standard.vert", "Assets/Shaders/Standard.frag", nullptr, "Standard");

	ResourceManager::LoadTexture("Assets/Textures/container.jpg", false, "container");
	ResourceManager::LoadTexture("Assets/Textures/awesomeface.png", true, "awesomeface");

	// -------------------------------------VAO/VBO------------------------------------------------------

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

void Actor::Draw()
{
	ResourceManager::GetShader("Standard").Use();

	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetTexture("container").Bind();
	glActiveTexture(GL_TEXTURE1);
	ResourceManager::GetTexture("awesomeface").Bind();

	ResourceManager::GetShader("Standard").SetInteger("texture1", 0);
	ResourceManager::GetShader("Standard").SetInteger("texture2", 1);
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Actor::Tick(float delta)
{
	auto trans =  glm::translate(model, glm::vec3(position[0], position[1], position[2]));
	
	trans = glm::rotate(trans, glm::radians(rotation[0]), glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, glm::radians(rotation[1]), glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, glm::radians(rotation[2]), glm::vec3(0, 0, 1));
	
	trans = glm::scale(trans, glm::vec3(scale[0], scale[1], scale[2]));
	
	ResourceManager::GetShader("Standard").Use();
	ResourceManager::GetShader("Standard").SetMatrix4("model", trans);
	ResourceManager::GetShader("Standard").SetMatrix4("view", view);
	ResourceManager::GetShader("Standard").SetMatrix4("projection", projection);
}
