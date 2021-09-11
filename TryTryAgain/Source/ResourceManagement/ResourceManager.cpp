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
#include <memory>

#include "Resource.h"

#include <filesystem>

namespace rm
{
    // Instantiate static variables
    std::map<std::string, Texture2D>    ResourceManager::Textures;

    std::map<std::string, std::weak_ptr<Resource>>        ResourceManager::Resources;

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

    void ResourceManager::Clear()
    {
        // (properly) delete all textures
        for (auto& iter : Textures)
            glDeleteTextures(1, &iter.second.ID);
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
