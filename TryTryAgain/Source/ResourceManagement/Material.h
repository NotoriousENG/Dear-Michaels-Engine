#pragma once
#include <vector>
#include <string>
#include <memory>
#include <cstddef>

namespace rm {

	class Shader;

	struct Uniform
	{
		Uniform(std::string name, int type)
		{
			this->name = name;
			this->type = type;
		}
		std::string name;
		int type;
		std::byte* data;
	};

	class Material
	{
	public:
		std::shared_ptr<rm::Shader> Shader;

		std::vector<Uniform> Uniforms;

		void InitUniforms();

		void ShowInspector();

		void SetUniforms();
	};
};
