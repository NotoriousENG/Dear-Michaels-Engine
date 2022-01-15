/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "Texture.h"
#include <glad/glad.h>
#include <stb_image.h>
// #include <Panels/Console.h>

namespace rm
{
    Texture2D::Texture2D()
        : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
    {
        glGenTextures(1, &this->ID);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &this->ID);
    }

    void Texture2D::Init(std::string path)
    {
        this->path = path;
        // load image
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            if (nrChannels == 1)
                Image_Format = GL_RED;
            else if (nrChannels == 3)
                Image_Format = GL_RGB;
            else if (nrChannels == 4)
                Image_Format = GL_RGBA;

            if (path.find(".png") != std::string::npos)
                Internal_Format = GL_RGBA;

            // now generate texture
            Generate(width, height, data);
        }
        else
        {
            printf("Texture failed to load at path %s", path.c_str());
        }
        // and finally free image data
        stbi_image_free(data);
    }

    void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
    {
        this->Width = width;
        this->Height = height;
        // create Texture
        glBindTexture(GL_TEXTURE_2D, this->ID);
        glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // set Texture wrap and filter modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, this->ID);
    }

}

