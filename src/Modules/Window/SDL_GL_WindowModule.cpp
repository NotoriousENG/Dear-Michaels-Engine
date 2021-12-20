#include "SDL_GL_WindowModule.h"

#ifdef EDITOR
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#endif //EDITOR

void SDL_GL_WindowModule::Init(bool* bQuit, GL_RenderModule* renderModule)
{
    // store quit ptr
    this->bQuit = bQuit;
    this->renderModule = renderModule;

    // Initialize SDL ko
    const char* caption = "Unreal Engine 7.1";
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdl_die("Couldn't initialize SDL");
    atexit(SDL_Quit);
    
    // Initialize SDL_GL
    SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

    // Request an OpenGL 4.5 context (should be core)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // Enable hardware or software rendering
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    // Also request a stencil buffer
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // Request a debug context.
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
    );

    // Create the window
    if (SCREEN_FULLSCREEN) {
        window = SDL_CreateWindow(
            caption,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
        );
    }
    else {
        window = SDL_CreateWindow(
            caption,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
        );
    }
    if (window == NULL) sdl_die("Couldn't set video mode");

    maincontext = SDL_GL_CreateContext(window);
    if (maincontext == NULL)
        sdl_die("Failed to create OpenGL context");
        
    // Use v-sync
    SDL_GL_SetSwapInterval(1);
}

void SDL_GL_WindowModule::Update()
{
    while (SDL_PollEvent(&event)) {
        // Forward to Imgui
#ifdef EDITOR
        ImGui_ImplSDL2_ProcessEvent(&event);
#endif //EDITOR

        switch (event.type)
        {
        case SDL_QUIT:
            *bQuit = true;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                renderModule->framebuffer_size_callback(event.window.data1, event.window.data2);
            }
            break;
        }
    }

    SDL_GL_SwapWindow(window);
}

void SDL_GL_WindowModule::Shutdown()
{
    SDL_GL_DeleteContext(maincontext);
    SDL_Quit();
}

void SDL_GL_WindowModule::GetRendererParams(void*& proc, int& w, int& h)
{
    SDL_GetWindowSize(window, &w, &h);

    proc = (void*)SDL_GL_GetProcAddress;
}

void SDL_GL_WindowModule::sdl_die(const char* message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    exit(2);
}