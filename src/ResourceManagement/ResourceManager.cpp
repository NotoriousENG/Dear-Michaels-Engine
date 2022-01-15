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
// #include "Panels/Console.h"
#include <memory>

#include "Resource.h"

#include <filesystem>

namespace rm
{
    // Instantiate static variables

    std::map<std::string, std::weak_ptr<Resource>>        ResourceManager::Resources;

    void ResourceManager::Clear()
    {
        
    }
}
