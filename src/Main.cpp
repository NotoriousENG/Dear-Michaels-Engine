#include <Modules/Window/SDL_GL_WindowModule.h>

#ifdef main
	#undef main
#endif // main

#ifdef EDITOR
#include <Modules/Editor/EditorModule.h>
#endif // EDITOR

#include "Scene/Scene.h"
#include "Scene/Entity.h"

int main(void)
{
	bool bQuit = false;

	// Initialize Modules
	SDL_GL_WindowModule winModule;
	GL_RenderModule renModule;

	winModule.Init(&bQuit, &renModule);

    {
		void* proc;
		int w;
		int h;
		winModule.GetRendererParams(proc, w, h);
		renModule.Init(proc, w, h);
	}

    Scene s;

    s.CreateEntity("Poop");
    s.CreateEntity();

#ifdef EDITOR
	EditorModule editorModule;

	editorModule.Init(renModule.GetTextureColorBuffer());
#endif

	// Update
	while (!bQuit)
	{
		winModule.Update(); 
		renModule.Update();

#ifdef EDITOR
		editorModule.Update();
#endif

	}

	// Shutdown
	winModule.Shutdown();
	renModule.Shutdown();

#ifdef EDITOR
	editorModule.Shutdown();
#endif

	return 0;
}
