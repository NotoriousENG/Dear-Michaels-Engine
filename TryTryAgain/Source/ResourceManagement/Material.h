#pragma once
#include <vector>
#include <string>
#include <memory>
#include <cstddef>

#include "Resource.h"

namespace rm {

	class Shader;
	class Texture2D;

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

	class Material : public Resource
	{
	public:

		std::shared_ptr<rm::Shader> Shader;

		std::vector<std::shared_ptr<rm::Texture2D>> Textures;

		std::vector<Uniform> Uniforms;

		void InitUniforms();

		void ShowInspector();

		void SetUniforms();

		std::string name = "Material";

		std::string ShaderPath = "Assets/Shaders/LoadModel.glsl";
	};
};
