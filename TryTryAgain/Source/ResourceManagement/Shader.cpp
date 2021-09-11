/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "Shader.h"

#include <iostream>
#include <sstream>
#include <fstream>

namespace rm
{
    Shader::~Shader()
    {
        glDeleteProgram(this->ID);
    }
    Shader& Shader::Use()
    {
        glUseProgram(this->ID);
        return *this;
    }

    void Shader::Init(std::string path)
    {
        // Compile Shader
        unsigned int sVertex, sFragment, sGeometry;
        
        //load file
        std::ifstream t(path);
        std::stringstream buf;
        buf << t.rdbuf();
        std::string src = buf.str();
        t.close();

        bool hasVertex = src.find("#ifdef VERTEX_SHADER") != std::string::npos;
        bool hasFragment = src.find("#ifdef FRAGMENT_SHADER") != std::string::npos;
        bool hasGeometry = src.find("#ifdef GEOMETRY_SHADER") != std::string::npos;

        this->ID = glCreateProgram();

        if (hasVertex)
        {
            sVertex = glCreateShader(GL_VERTEX_SHADER);
            std::string vstr = "#version 330\n#define VERTEX_SHADER\n" + src;
            const char* vertexSource = vstr.c_str();
            glShaderSource(sVertex, 1, &vertexSource, NULL);
            glCompileShader(sVertex);
            checkCompileErrors(sVertex, "VERTEX " + path);
            glAttachShader(this->ID, sVertex);
        }

        if (hasFragment)
        {
            sFragment = glCreateShader(GL_FRAGMENT_SHADER);
            std::string fstr = "#version 330\n#define FRAGMENT_SHADER\n" + src;
            const char* fragmentSource = fstr.c_str();
            glShaderSource(sFragment, 1, &fragmentSource, NULL);
            glCompileShader(sFragment);
            checkCompileErrors(sFragment, "FRAGMENT " + path);
            glAttachShader(this->ID, sFragment);
        }

        if (hasGeometry)
        {
            sGeometry = glCreateShader(GL_GEOMETRY_SHADER);
            std::string gstr = "#version 330\n#define GEOMETRY_SHADER\n" + src;
            const char* geometrySource = gstr.c_str();
            glShaderSource(sGeometry, 1, &geometrySource, NULL);
            glCompileShader(sGeometry);
            checkCompileErrors(sGeometry, "GEOMETRY " + path);
            glAttachShader(this->ID, sGeometry);
        }
            
        glLinkProgram(this->ID);
        checkCompileErrors(this->ID, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary
        if (hasVertex)
            glDeleteShader(sVertex);
        if (hasFragment)
            glDeleteShader(sFragment);
        if (hasGeometry)
            glDeleteShader(sGeometry);
    }

    void Shader::SetFloat(const char* name, float value, bool useShader)
    {
        if (useShader)
            this->Use();
        glUniform1f(glGetUniformLocation(this->ID, name), value);
    }
    void Shader::SetInteger(const char* name, int value, bool useShader)
    {
        if (useShader)
            this->Use();
        glUniform1i(glGetUniformLocation(this->ID, name), value);
    }
    void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
    {
        if (useShader)
            this->Use();
        glUniform2f(glGetUniformLocation(this->ID, name), x, y);
    }
    void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
    {
        if (useShader)
            this->Use();
        glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
    }
    void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
    {
        if (useShader)
            this->Use();
        glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
    }
    void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
    {
        if (useShader)
            this->Use();
        glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
    }
    void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
    {
        if (useShader)
            this->Use();
        glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
    }
    void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
    {
        if (useShader)
            this->Use();
        glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
    }
    void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
    {
        if (useShader)
            this->Use();
        glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
    }


    void Shader::checkCompileErrors(unsigned int object, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- "
                    << std::endl;
            }
        }
        else
        {
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- "
                    << std::endl;
            }
        }
    }
}