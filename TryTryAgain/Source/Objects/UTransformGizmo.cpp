#include "UTransformGizmo.h"


#include "Elements/Game.h"
#include "ResourceManagement/ResourceManager.h"

#include <ResourceManagement/Shader.h>

UTransformGizmo::UTransformGizmo()
{
	model = glm::mat4(1);

	this->transform = FTransform();

	Shader = rm::ResourceManager::Load<rm::Shader>("Assets/Shaders/Gizmo.glsl");

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
	Shader->Use();

	Shader->Use();
	Shader->SetMatrix4("model", transform.GetModelMatrixWorld());
	Shader->SetMatrix4("view", Camera::Main.view);
	Shader->SetMatrix4("projection", Camera::Main.projection);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 6);
}
