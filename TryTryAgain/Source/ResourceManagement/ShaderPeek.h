#pragma once
#include <vector>
#include <string>
#include <memory>
#include <cstddef>

namespace rm {
	class Shader;
}

struct Property
{
	Property(std::string name, int type)
	{
		this->name = name;
		this->type = type;
	}
	std::string name;
	int type;
	std::byte* data;
};

class ShaderPeek
{
public:
	std::shared_ptr<rm::Shader> Shader;

	std::vector<Property> Properties;

	void GetProperties();

	void Inspect();
};

