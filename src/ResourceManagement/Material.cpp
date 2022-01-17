#include "Material.h"
#include "ResourceManagement/Shader.h"

#include "Texture.h"
#include "Shader.h"
#include "Resource.h"
#include <ResourceManagement/ResourceManager.h>


namespace rm {

	void Material::InitUniforms()
	{
		Uniforms.clear();

		Shader->Use();

		GLint numUniforms = 0;
		glGetProgramInterfaceiv(Shader->ID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
		const GLenum properties[4] = { GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION };

		for (int unif = 0; unif < numUniforms; ++unif)
		{
			GLint values[4];
			glGetProgramResourceiv(Shader->ID, GL_UNIFORM, unif, 4, properties, 4, NULL, values);

			// Skip any uniforms that are in a block.
			if (values[0] != -1)
				continue;

			// Get the name. Must use a std::vector rather than a std::string for C++03 standards issues.
			// C++11 would let you use a std::string directly.
			std::vector<char> nameData(values[2]);
			glGetProgramResourceName(Shader->ID, GL_UNIFORM, unif, nameData.size(), NULL, &nameData[0]);
			std::string name(nameData.begin(), nameData.end() - 1);

			auto type = values[1];

			Uniforms.push_back(Uniform(name, type));
			auto* p = &Uniforms.back();
			switch (type)
			{
			case GL_FLOAT:
				p->data = new std::byte[sizeof(float)];
				break;
			case GL_FLOAT_VEC4:
			{
				p->data = new std::byte[sizeof(glm::vec4)];
				float f[] = { 1,1,1,1 };
				memcpy(p->data, f, 4 * sizeof(float));
				Shader->SetVector4f(p->name.c_str(), glm::vec4(f[0], f[1], f[2], f[3]));
			}
				break;
			case GL_BOOL:
				p->data = new std::byte[sizeof(bool)];
				break;

			}
		}
	}

	void Material::SetUniforms()
	{
		Shader->Use();
		for (auto& p : Uniforms)
		{
			switch (p.type)
			{
			case GL_FLOAT:
			{
				auto t = *(float*)p.data;
				Shader->SetFloat(p.name.c_str(), t);
			}
			break;
			case GL_FLOAT_VEC4:
			{
				auto* t = (float*)p.data;
				Shader->SetVector4f(p.name.c_str(), glm::vec4(t[0], t[1], t[2], t[3]));
			}
			break;
			case GL_BOOL:
			{
				auto t = *(bool*)p.data;
				Shader->SetBool(p.name.c_str(), t);
			}
			break;
			}
		}
	}
}

