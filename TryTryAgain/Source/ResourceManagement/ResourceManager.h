#pragma once

#include <map>
#include <string>

#include <glad/glad.h>
#include "Texture.h"
#include <memory>

namespace rm
{   
    class ResourceManager
    {
    public:
        // resource storage
        static std::map<std::string, Texture2D> Textures;

        static std::map<std::string, std::weak_ptr<Resource>>        Resources;

        template <typename T>
        static std::shared_ptr<T> Load(std::string file)
        {
            static_assert(std::is_base_of<Resource, T>::value, "Resource Types must derive from Resource");

            if (Resources.find(file) == Resources.end() || Resources[file].expired())
            {
                std::shared_ptr<Resource> sp = std::make_shared<T>();
                sp->Init(file);
                Resources[file] = sp;
                return std::dynamic_pointer_cast<T>(Resources[file].lock());
            }

            return std::dynamic_pointer_cast<T>(Resources[file].lock());
        }

        // loads (and generates) a texture from file
        static Texture2D* LoadTexture(const char* file, bool alpha, std::string name);
        // retrieves a stored texture
        static Texture2D* GetTexture(std::string name);

        // properly de-allocates all loaded resources
        static void      Clear();

    private:
        // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
        ResourceManager() { }

        // loads a single texture from file
        static Texture2D loadTextureFromFile(const char* file, bool alpha);
    };
}
