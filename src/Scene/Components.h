#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <ResourceManagement/Model.h>

struct NameComponent
{
    std::string Name;
};

struct TransformComponent
{
    glm::mat4 transform = glm::mat4(1.0f);
};

struct StaticMeshComponent
{
    StaticMeshComponent()
    {
        this->ModelPath = "Assets/Models/cube/cube.obj";
        this->ShaderPath = "Assets/Shaders/LoadModel.glsl";
        this->Model = std::make_shared<rm::Model>();
        this->Model->Init(ModelPath);
    }
    std::shared_ptr<rm::Model> Model;
    std::string ModelPath = "Assets/Models/cube/cube.obj";
    std::string ShaderPath = "Assets/Shaders/LoadModel.glsl";
    bool bDraw = true;
};
