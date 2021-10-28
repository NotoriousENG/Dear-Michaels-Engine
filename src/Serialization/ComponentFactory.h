#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <string>

class UComponent;
class ComponentFactory
{
public:

	static ComponentFactory instance;

	typedef std::function<std::unique_ptr<UComponent>()> CreationFunction;

	ComponentFactory();

	std::unique_ptr<UComponent> Create(std::string CompName);

	std::vector<const char*> Keys;

private:

	void Register(std::string CompName, CreationFunction cFunc);

	std::unordered_map<std::string, CreationFunction> registry;
};

