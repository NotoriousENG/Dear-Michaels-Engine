#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>

struct NameComponent
{
    std::string Name;
};

struct TransformComponent
{
    glm::mat4 transform;
};

struct StaticMeshComponent
{
    std::shared_ptr<int> MeshHandle;
    std::string ModelPath = "Assets/Models/cube/cube.obj";
    std::string ShaderPath = "Assets/Shaders/LoadModel.glsl";
};
