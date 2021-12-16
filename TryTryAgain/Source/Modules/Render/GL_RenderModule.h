#pragma once

#include <SDL2/SDL.h>
#include <glad/glad.h>

class GL_RenderModule
{
public:

	void Init(void* __cdecl proc, int w, int h);

	void Update();
	void Shutdown();

	void framebuffer_size_callback(int width, int height);

	unsigned int GetTextureColorBuffer();

private:

	unsigned int framebuffer;
	unsigned int textureColorbuffer;

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

