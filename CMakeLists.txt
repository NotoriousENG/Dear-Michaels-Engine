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

find_package(EnTT CONFIG REQUIRED)

# CUSTOM FIND SCRIPTS IN CMAKE DIRECTORY
find_package(GLM REQUIRED)

add_compile_definitions(EDITOR)

add_executable(Dear-Michaels-Engine 
    # Engine
    "src/Main.cpp"

    # Scene
    "src/Scene/Scene.cpp"
    "src/Scene/Entity.cpp"

    # Editors
    "src/Scene/Editor/SceneEditor.cpp"

    # Modules ------------------------------------
    "src/Modules/Editor/EditorModule.cpp"

    "src/Modules/Render/GL_RenderModule.cpp"

    "src/Modules/Window/SDL_GL_WindowModule.cpp"
    # --------------------------------------------

    # Implementation .c/cpp
    "src/stb_image.cpp" 

    # Third Party .c/.cpp
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
    "src/ThirdParty/glad/glad.c"
)

# ADD ALL OF OUR 3RD PARTY LIBS AND INCLUDES

target_include_directories(Dear-Michaels-Engine PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine PRIVATE ${SDL2_LIBRARIES})

target_include_directories(Dear-Michaels-Engine PRIVATE ${GLM_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine PRIVATE ${GLM_LIBRARIES})

target_include_directories(Dear-Michaels-Engine PRIVATE ${CEREAL_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine PRIVATE ${CEREAL_LIBRARIES})

target_include_directories(Dear-Michaels-Engine PRIVATE ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(Dear-Michaels-Engine PRIVATE ${ASSIMP_LIBRARIES})

target_link_libraries(Dear-Michaels-Engine PRIVATE EnTT::EnTT)

# needed for glad
target_link_libraries(Dear-Michaels-Engine PRIVATE ${CMAKE_DL_LIBS})

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



# COPY ASSETS AND REsrcS FOLDERS
add_custom_target(copy_resrcs
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources
)
add_dependencies(Dear-Michaels-Engine copy_resrcs)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets
)
add_dependencies(Dear-Michaels-Engine copy_assets)