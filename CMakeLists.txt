cmake_minimum_required(VERSION 3.7)

# ADD OUR CMAKE DIRECTORY, NEEDED FOR GLM AND GLAD
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(Dear-Michaels-Engine)

# C++20 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ENSURE WE CAN FIND OUR DEPENDENCIES

# CMAKE SHIPS WITH THESE FIND SCRIPTS
find_package(SDL2 REQUIRED)
find_package(CEREAL REQUIRED)
find_package(ASSIMP REQUIRED)

# CUSTOM FIND SCRIPTS IN CMAKE DIRECTORY
find_package(GLM REQUIRED)

add_executable(Dear-Michaels-Engine 
    # CPP
    "src/ResourceManagement/Material.cpp" 
    "src/Panels/ContentBrowser.cpp" 
    "src/ResourceManagement/Skybox.cpp" 
    "src/Input/Input.cpp" 
    "src/Serialization/ComponentFactory.cpp" 
    "src/Panels/Inspector.cpp" 
    "src/ResourceManagement/Model.cpp" 
    "src/Application.cpp" 
    "src/Elements/Camera.cpp" 
    "src/Elements/Game.cpp" 
    "src/Actors/AActor.cpp" 
    "src/Panels/GameWindow.cpp" 
    "src/Panels/Hierarchy.cpp" 
    "src/ResourceManagement/Mesh.cpp" 
    "src/ResourceManagement/ResourceManager.cpp" 
    "src/ResourceManagement/Shader.cpp" 
    "src/ResourceManagement/Texture.cpp" 
    "src/Main.cpp" 
    "src/Panels/Console.cpp" 
    "src/stb_image.cpp" 
    "src/ThirdParty/imgui-docking/backends/imgui_impl_opengl3.cpp" 
    "src/ThirdParty/imgui-docking/backends/imgui_impl_sdl.cpp" 
    "src/ThirdParty/imgui-docking/imgui.cpp" 
    "src/ThirdParty/imgui-docking/imgui_demo.cpp" 
    "src/ThirdParty/imgui-docking/imgui_draw.cpp" 
    "src/ThirdParty/imgui-docking/imgui_tables.cpp" 
    "src/ThirdParty/imgui-docking/imgui_widgets.cpp" 
    "src/ThirdParty/imgui-docking/misc/cpp/imgui_stdlib.cpp" 
    "src/ThirdParty/ImGuiFileDialog/ImGuiFileDialog.cpp" 
    "src/ThirdParty/imguizmo/ImCurveEdit.cpp" 
    "src/ThirdParty/imguizmo/ImGradient.cpp" 
    "src/ThirdParty/imguizmo/ImGuizmo.cpp" 
    "src/ThirdParty/imguizmo/ImSequencer.cpp" 
    "src/Window/Window.cpp" 
    "src/Objects/UTransformGizmo.cpp" 
    "src/Components/UComponent.cpp" 
    "src/Components/UStaticModelComponent.cpp" 
    "src/Objects/UObject.cpp" 
    "src/Components/UPlayerControlComponent.cpp" 
    "src/Components/UCameraComponent.cpp" 
    "src/ThirdParty/glad/glad.c"

    # HEADERS

    "src/ResourceManagement/Resource.h" 
    "src/ResourceManagement/Material.h" 
    "src/Panels/ContentBrowser.h" 
    "src/Panels/IPanel.h" 
    "src/ResourceManagement/Skybox.h" 
    "src/Input/Input.h" 
    "src/Serialization/ComponentFactory.h" 
    "src/Panels/Inspector.h" 
    "src/ResourceManagement/Model.h" 
    "src/Application.h" 
    "src/Elements/Camera.h" 
    "src/Elements/Game.h" 
    "src/Actors/AActor.h" 
    "src/ResourceManagement/Mesh.h" 
    "src/ResourceManagement/ResourceManager.h" 
    "src/ResourceManagement/Shader.h" 
    "src/ResourceManagement/Texture.h" 
    "src/Panels/Console.h" 
    "src/Panels/GameWindow.h" 
    "src/Panels/Hierarchy.h" 
    "src/Serialization/CerealGLM.h" 
    "src/Structs/FString.h" 
    "src/Structs/FTransform.h" 
    "src/Utility/Utility.h" 
    "src/Window/Window.h" 
    "src/Objects/UTransformGizmo.h" 
    "src/Components/UComponent.h" 
    "src/Components/UStaticModelComponent.h" 
    "src/Objects/UObject.h" 
    "src/Components/UPlayerControlComponent.h" 
    "src/Components/UCameraComponent.h" 
)

# ADD ALL OF OUR 3RD PARTY LIBS AND INCLUDES

target_include_directories(Dear-Michaels-Engine PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine ${SDL2_LIBRARIES})

target_include_directories(Dear-Michaels-Engine PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine ${GLM_LIBRARIES})

target_include_directories(Dear-Michaels-Engine PRIVATE ${CEREAL_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine ${CEREAL_LIBRARIES})

target_include_directories(Dear-Michaels-Engine PRIVATE ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine ${ASSIMP_LIBRARIES})

# needed for glad
target_link_libraries(Dear-Michaels-Engine ${CMAKE_DL_LIBS})

# LOCAL INCLUDES
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")

target_include_directories(
    Dear-Michaels-Engine
    PUBLIC
    ${header_dir_list} # Recursive
)



# COPY ASSETS AND RESOURCES FOLDERS
add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources
)
add_dependencies(Dear-Michaels-Engine copy_resources)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets
)
add_dependencies(Dear-Michaels-Engine copy_assets)