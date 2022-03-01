#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <entt/entt.hpp>

class ComponentRegistry
{
public:
	inline static std::unordered_map<std::string, entt::hashed_string> registeredComponents;

	static void Init();

	static std::vector<std::string> GetComponentNames();

	static void LogRegisteredComponents();
};