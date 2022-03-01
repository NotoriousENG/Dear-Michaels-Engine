cmake_minimum_required(VERSION 3.7)

# ADD OUR CMAKE DIRECTORY, NEEDED FOR GLM AND GLAD
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(Dear-Michaels-Engine)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ENSURE WE CAN FIND OUR DEPENDENCIES

find_package(SDL2 REQUIRED)
# find_package(CEREAL REQUIRED)

find_package(assimp CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

# EnTT must be at least 3.9.0
message("-- EnTT VERSION FOUND: " ${EnTT_VERSION})
message("-- EnTT VERSION EXPECTED: 3.9.0")

# CUSTOM FIND SCRIPTS IN CMAKE DIRECTORY
find_package(GLM REQUIRED)

set(EDITOR true CACHE BOOL "Enable edtitor windows and tools developed with imgui")

add_executable(Dear-Michaels-Engine 
    # Engine
    "src/Main.cpp"

    # Scene
    "src/Scene/Scene.cpp"
    "src/Scene/Entity.cpp"
    "src/Scene/ComponentRegistry.cpp"

    # Camera
    "src/Elements/Camera.cpp"

    # Modules ------------------------------------

    "src/Modules/Render/GL_RenderModule.cpp"

    "src/Modules/Window/SDL_GL_WindowModule.cpp"
    # --------------------------------------------

    # Input
    "src/Input/Input.cpp"

    # Implementation .c/cpp
    "src/stb_image.cpp" 

    # Third Party .c/.cpp
    "src/ThirdParty/glad/glad.c"

    # Resources
    "src/ResourceManagement/Material.cpp"
    "src/ResourceManagement/Mesh.cpp"
    "src/ResourceManagement/Model.cpp"
    "src/ResourceManagement/ResourceManager.cpp"
    "src/ResourceManagement/Shader.cpp"
    # "src/ResourceManagement/Skybox.cpp"
    "src/ResourceManagement/Texture.cpp"
)

target_include_directories(
    Dear-Michaels-Engine

    PUBLIC "src"
    PUBLIC "src/Modules/Render"
    PUBLIC "src/Modules/Window"

    # Input
    PUBLIC "src/Input/"
    
    # Scene
    PUBLIC "src/Scene"
    PUBLIC "src/Scene/Systems"

    # Camera
    PUBLIC "src/Elements"


    # Third Party
    PUBLIC "src/ThirdParty"
    PUBLIC "src/ThirdParty/glad"

    # Resources
    PUBLIC "src/ResourceManagement"
)

if (${EDITOR})

    add_compile_definitions(EDITOR)

    target_sources(Dear-Michaels-Engine
        PRIVATE "src/Modules/Editor/EditorModule.cpp"
        PRIVATE "src/Scene/Editor/SceneEditor.cpp"
        PRIVATE "src/Scene/Editor/Inspector.cpp"

        # Third Party
        PRIVATE "src/ThirdParty/imgui-docking/backends/imgui_impl_opengl3.cpp" 
        PRIVATE "src/ThirdParty/imgui-docking/backends/imgui_impl_sdl.cpp" 
        PRIVATE "src/ThirdParty/imgui-docking/imgui.cpp" 
        PRIVATE "src/ThirdParty/imgui-docking/imgui_demo.cpp" 
        PRIVATE "src/ThirdParty/imgui-docking/imgui_draw.cpp" 
        PRIVATE "src/ThirdParty/imgui-docking/imgui_tables.cpp" 
        PRIVATE "src/ThirdParty/imgui-docking/imgui_widgets.cpp" 
        PRIVATE "src/ThirdParty/imgui-docking/misc/cpp/imgui_stdlib.cpp" 
        PRIVATE "src/ThirdParty/ImGuiFileDialog/ImGuiFileDialog.cpp" 
        PRIVATE "src/ThirdParty/imguizmo/ImCurveEdit.cpp" 
        PRIVATE "src/ThirdParty/imguizmo/ImGradient.cpp" 
        PRIVATE "src/ThirdParty/imguizmo/ImGuizmo.cpp" 
        PRIVATE "src/ThirdParty/imguizmo/ImSequencer.cpp" 
    )

    target_include_directories(Dear-Michaels-Engine
        PUBLIC "src/Modules/Editor"
        PUBLIC "src/Scene/Editor"

        # Third Party
        PUBLIC "src/ThirdParty/imgui-docking"
        PUBLIC "src/ThirdParty/imgui-docking/misc/cpp"
        PUBLIC "src/ThirdParty/imgui-docking/backends"
        PUBLIC "src/ThirdParty/ImGuiFileDialog"
        PUBLIC "src/ThirdParty/imguizmo"
    )

    if (WIN32)
        target_include_directories(Dear-Michaels-Engine

            # Third Party
            PUBLIC "src/ThirdParty/ImGuiFileDialog/dirent/dirent.h"
        )
    endif()
endif()


# ADD ALL OF OUR 3RD PARTY LIBS AND INCLUDES

target_include_directories(Dear-Michaels-Engine PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine PRIVATE SDL2::SDL2 SDL2::SDL2main)

target_include_directories(Dear-Michaels-Engine PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine PRIVATE ${GLM_LIBRARIES})

target_include_directories(Dear-Michaels-Engine PRIVATE ${CEREAL_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine PRIVATE ${CEREAL_LIBRARIES})

target_include_directories(Dear-Michaels-Engine PRIVATE ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine PRIVATE  assimp::assimp)

target_link_libraries(Dear-Michaels-Engine PRIVATE EnTT::EnTT)

# needed for glad
target_link_libraries(Dear-Michaels-Engine PRIVATE ${CMAKE_DL_LIBS})

# COPY ASSETS AND RESOURCESS FOLDERS
add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources
)
add_dependencies(Dear-Michaels-Engine copy_resources)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets
)
add_dependencies(Dear-Michaels-Engine copy_assets)
