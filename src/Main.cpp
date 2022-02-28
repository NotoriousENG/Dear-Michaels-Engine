#include <Modules/Window/SDL_GL_WindowModule.h>

#ifdef main
	#undef main
#endif // main

#ifdef EDITOR
#include <Modules/Editor/EditorModule.h>
#endif // EDITOR

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"

int main(void)
{
	TransformComponent::RegisterComponent();

	Scene s;

	auto entity = s.CreateEntity("Mario");

	auto type = entt::resolve(entt::hashed_string("TransformComponent"));

	if (auto func = type.func(entt::hashed_string("assign")))
	{
		func.invoke({}, entity);
	}
	
	// TransformComponent::assign(entity);
	
	// type.func(entt::hashed_string("assign")).invoke(registry, entity);

	auto view = s.registry.view<TransformComponent>();

	// use forward iterators and get only the components of interest
	for (auto entity : view) {
		auto& t = view.get<TransformComponent>(entity);
		printf("TransformComponent\n");
	}

	return 0;

//	bool bQuit = false;
//
//	// Initialize Modules
//	SDL_GL_WindowModule winModule;
//	GL_RenderModule renModule;
//
//	winModule.Init(&bQuit, &renModule);
//
//    {
//		void* proc;
//		int w;
//		int h;
//		winModule.GetRendererParams(proc, w, h);
//
//		renModule.Init(proc, w, h);
//	}
//
//    Scene s;
//
//    auto e = s.CreateEntity("Mario");
//    e.AddComponent<TransformComponent>();
//	e.AddComponent<StaticMeshComponent>();
//
//    s.CreateEntity();
//
//#ifdef EDITOR
//	EditorModule editorModule;
//
//	editorModule.Init(&renModule);
//#endif
//
//	// Update
//	while (!bQuit)
//	{
//		winModule.Update(); 
//		renModule.Update();
//
//#ifdef EDITOR
//		editorModule.Update();
//#endif
//
//	}
//
//	// Shutdown
//	winModule.Shutdown();
//	renModule.Shutdown();
//
//#ifdef EDITOR
//	editorModule.Shutdown();
//#endif

	return 0;
}
