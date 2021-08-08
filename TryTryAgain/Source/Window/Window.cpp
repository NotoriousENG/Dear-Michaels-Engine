#include "Window.h"

#include <imgui_impl_opengl3.h>
#include <imguizmo/ImGuizmo.h>

#include "Panels/Hierarchy.h"

#include <ImGuiFileDialog.h>
#include <ThirdParty/stb_image.h>

void Window::sdl_die(const char* message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    exit(2);
}

void APIENTRY Window::openglCallbackFunction(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
) {
    (void)source; (void)type; (void)id;
    (void)severity; (void)length; (void)userParam;
    fprintf(stderr, "%s\n", message);
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        fprintf(stderr, "Aborting...\n");
        abort();
    }
}

void Window::init_screen(const char* caption) {
    // Initialize SDL 
    if (SDL_Init(SDL_INIT_VIDEO | GL_STENCIL_BITS) < 0)
        sdl_die("Couldn't initialize SDL");
    atexit(SDL_Quit);
    SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

    // Request an OpenGL 4.5 context (should be core)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
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

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // Enable the debug callback
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);
    glDebugMessageControl(
        GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_TRUE);

    // Use v-sync
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
}

void Window::init_imgui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, maincontext);
    ImGui_ImplOpenGL3_Init();
}

void Window::execute() {
    init_screen("Unreal Engine 7");
    init_imgui();
    ImGuiIO& io = ImGui::GetIO();
    SDL_Event event;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    
    gameWindow = std::make_unique<Panels::GameWindow>();

    hierarchy = std::make_unique<Panels::Hierarchy>(gameWindow->MyGame.get());
	
    while (!quit) {
        gameWindow->MyGame->mouseRel = glm::vec2(0, 0);

        for (int i = 0; i < 6; i++)
        {
            gameWindow->MyGame->MouseButtonsUp[i] = false;
        }
    	
        while (SDL_PollEvent(&event)) {
            // Forward to Imgui
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    framebuffer_size_callback(event.window.data1, event.window.data2);
                }
                break;
            }
            processInputForWindow(event);
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        ImGui::DockSpaceOverViewport();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ShowAppMainMenuBar();

        ShowFileDialog();

        ShowConsole(&show_console);

        gameWindow->Draw();

        hierarchy->Draw(&show_hierarchy);


        // Rendering
        ImGui::Render();
        /*glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);*/
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        SDL_GL_SwapWindow(window);
    }

    // Cleanup imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Cleanup SDL
    SDL_Quit();
}

void Window::processInputForWindow(SDL_Event event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        gameWindow->MyGame->playing = false;

    auto key = event.key.keysym.sym;
    if (key >= 0 && key < 1024)
    {
        if (event.type == SDL_KEYDOWN)
            gameWindow->MyGame->Keys[key] = true;
        else if (event.type == SDL_KEYUP)
            gameWindow->MyGame->Keys[key] = false;
    }
	
	if (event.type == SDL_MOUSEMOTION)
	{
		gameWindow->MyGame->mouseRel = glm::vec2(event.motion.xrel, event.motion.yrel);
        gameWindow->MyGame->mouseMoving = true;
	}

    auto button = event.button.button;
	if (button >= 0 && button < 6)
	{
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
            gameWindow->MyGame->MouseButtons[button] = true;
            gameWindow->MyGame->MouseButtonsDown[button] = true;
		}
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            gameWindow->MyGame->MouseButtons[button] = false;
            gameWindow->MyGame->MouseButtonsUp[button] = true;
        }
        else
        {
            gameWindow->MyGame->MouseButtonsDown[button] = false;
            gameWindow->MyGame->MouseButtonsUp[button] = false;
        }
	}

	if (event.type == SDL_KEYDOWN)
	{
		if (key == SDLK_F5)
		{
            gameWindow->playing = true;
            gameWindow->MyGame->playing = true;
            SDL_SetRelativeMouseMode(SDL_TRUE);
		}
	}
}

void Window::framebuffer_size_callback(int width, int height)
{
    glViewport(0, 0, width, height);

}

void Window::ShowAppMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load Scene", ""))
            {
                ImGuiFileDialog::Instance()->OpenDialog("Open Scene", "Open Scene File", ".json", "Assets/Scenes/");
            }
            
            if (ImGui::MenuItem("Save Scene", ""))
            {
                ImGuiFileDialog::Instance()->OpenDialog("Save Scene", "Save Scene File", ".json", "Assets/Scenes/");
            }

            ImGui::EndMenu();
        }
    	if (ImGui::BeginMenu("Window"))
    	{
            ImGui::MenuItem("Demo Window", "", &show_demo_window);
            ImGui::MenuItem("Console", "", &show_console);
            ImGui::MenuItem("Hierarchy", "", &show_hierarchy);

            ImGui::EndMenu();
    	}
        ImGui::EndMainMenuBar();
    }
}

void Window::ShowFileDialog()
{
    // display
    if (ImGuiFileDialog::Instance()->Display("Open Scene"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            gameWindow->MyGame->LoadScene(filePathName.c_str());
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }

    // display
    if (ImGuiFileDialog::Instance()->Display("Save Scene"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            gameWindow->MyGame->SaveScene(filePathName.c_str());
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

