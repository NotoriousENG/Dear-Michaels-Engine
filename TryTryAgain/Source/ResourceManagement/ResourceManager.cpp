/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"
#include "Panels/Console.h"
#include "Model.h"

namespace rm
{
    // Instantiate static variables
    std::map<std::string, Texture2D>    ResourceManager::Textures;
    std::map<std::string, Shader>       ResourceManager::Shaders;
    std::map<std::string, bool>         ResourceManager::ShadersLoaded;
    std::map<std::string, Mesh>         ResourceManager::Meshes;
    std::map<std::string, Model>        ResourceManager::Models;

    Shader* ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
    {
        if (ShadersLoaded[name])
        {
            // M_LOG("[warn] Shader: %s has already been loaded", name.c_str());
            return  &Shaders[name];
        }
        Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
        ShadersLoaded[name] = true;
        return &Shaders[name];
    }

    Shader* ResourceManager::GetShader(std::string name)
    {
        if (!ShadersLoaded[name])
        {
            M_LOG("[error] Shader: %s has not been loaded", name.c_str());
            abort();
        }
        return &Shaders[name];
    }

    Texture2D* ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
    {
        if (Textures.find(name) != Textures.end())
        {
            return &Textures[name];
        }
        Textures[name] = loadTextureFromFile(file, alpha);
        return &Textures[name];
    }

    Texture2D* ResourceManager::GetTexture(std::string name)
    {
        return &Textures[name];
    }

    Mesh* ResourceManager::CreateMesh(std::string name)
    {
        /*if (Meshes.find(name) != Meshes.end())
        {
            return &Meshes[name];
        }
        Meshes[name] = Mesh();
        return &Meshes[name];*/
        return nullptr;
    }

    Mesh* ResourceManager::GetMesh(std::string name)
    {
        return &Meshes[name];
    }

    Model* ResourceManager::LoadModel(std::string file, bool alpha)
    {
        if (Models.find(file) != Models.end())
        {
            return &Models[file];
        }
        rm::Model model;
        model.gammaCorrection = alpha;
        model.loadModel(file);
        Models[file] = model;
        return &Models[file];
    }

    Model* ResourceManager::GetModel(std::string file)
    {
        return &Models[file];
    }

    void ResourceManager::Clear()
    {
        // (properly) delete all shaders	
        for (auto& iter : Shaders)
            glDeleteProgram(iter.second.ID);
        // (properly) delete all textures
        for (auto& iter : Textures)
            glDeleteTextures(1, &iter.second.ID);
    }

    Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        try
        {
            // open files
            std::ifstream vertexShaderFile(vShaderFile);
            std::ifstream fragmentShaderFile(fShaderFile);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vertexShaderFile.rdbuf();
            fShaderStream << fragmentShaderFile.rdbuf();
            // close file handlers
            vertexShaderFile.close();
            fragmentShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if (gShaderFile != nullptr)
            {
                std::ifstream geometryShaderFile(gShaderFile);
                std::stringstream gShaderStream;
                gShaderStream << geometryShaderFile.rdbuf();
                geometryShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::exception e)
        {
            M_LOG("ERROR::SHADER: Failed to read shader files");
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        const char* gShaderCode = geometryCode.c_str();
        // 2. now create shader object from source code
        Shader shader;
        shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
        return shader;
    }

    Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
    {
        // create texture object
        Texture2D texture;
        if (alpha)
        {
            texture.Internal_Format = GL_RGBA;
            texture.Image_Format = GL_RGBA;
        }
        // load image
        int width, height, nrChannels;
        unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
        // now generate texture
        texture.Generate(width, height, data);
        // and finally free image data
        stbi_image_free(data);
        return texture;
    }
}
