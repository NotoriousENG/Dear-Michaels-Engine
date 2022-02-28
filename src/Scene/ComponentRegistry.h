#pragma once

#include <string>
#include <unordered_map>

#include <entt/entt.hpp>

class ComponentRegistry
{
public:
	inline static std::unordered_map<std::string, entt::hashed_string> registeredComponents;
};