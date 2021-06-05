#include "Mesh.h"
#include <ResourceManagement/ResourceManager.h>

namespace rm
{
	Mesh::Mesh()
	{
		// Resources
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

	void Mesh::DrawPicking(int id, glm::mat4 MVP)
	{
		// Convert id into unique color
		int r = (id & 0x000000FF) >> 0;
		int g = (id & 0x0000FF00) >> 8;
		int b = (id & 0x00FF0000) >> 16;

		rm::ResourceManager::GetShader("Picking")->SetMatrix4("MVP", MVP);
		rm::ResourceManager::GetShader("Picking")->SetVector4f("PickingColor", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.f));

		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}

	void Mesh::Draw(rm::Shader* shader)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}

}