#pragma once

#include <glad/glad.h>

namespace rm {
	class Mesh;
	class Shader;
}

class GL_RenderModule
{
public:

	void Init(void* proc, int w, int h);

	void Update();
	void Shutdown();

	unsigned int GetTextureColorBuffer();
	void FrameBufferResizeCallback(int w, int h);

private:

    void DrawMesh(rm::Mesh* mesh, rm::Shader* shader);

	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;

	int w;
	int h;

	static void APIENTRY openglCallbackFunction(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	);
};

