#include "UTransformGizmo.h"


#include "Elements/Game.h"
#include "ResourceManagement/ResourceManager.h"

UTransformGizmo::UTransformGizmo()
{
	model = glm::mat4(1);

	this->transform = FTransform
	{
		glm::vec3(0),
		glm::vec3(0),
		glm::vec3(1)
	};

	ResourceManagement::ResourceManager::LoadShader("Assets/Shaders/Gizmo.vert", "Assets/Shaders/Gizmo.frag", nullptr, "Gizmo");

	// -------------------------------------VAO/VBO------------------------------------------------------

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

FTransform UTransformGizmo::SetTransform()
{
	return FTransform();
}

FTransform UTransformGizmo::GetTransform()
{
	return FTransform();
}

void UTransformGizmo::OnDrag()
{
}

void UTransformGizmo::Draw()
{
	ResourceManagement::ResourceManager::GetShader("Gizmo").Use();

	auto trans = glm::translate(model, transform.position);

	trans = glm::rotate(trans, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));

	trans = glm::scale(trans, transform.scale);

	ResourceManagement::ResourceManager::GetShader("Gizmo").Use();
	ResourceManagement::ResourceManager::GetShader("Gizmo").SetMatrix4("model", trans);
	ResourceManagement::ResourceManager::GetShader("Gizmo").SetMatrix4("view", Game::MainCamera.view);
	ResourceManagement::ResourceManager::GetShader("Gizmo").SetMatrix4("projection", Game::MainCamera.projection);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 6);
}
