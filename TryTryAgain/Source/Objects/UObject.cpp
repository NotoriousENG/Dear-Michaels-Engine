#include "UObject.h"

std::unordered_map<std::string, UObject::create_f*>& UObject::Registry()
{
    static std::unordered_map<std::string, UObject::create_f*> impl;
    return impl;
}