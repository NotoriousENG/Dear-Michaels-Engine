#include "ShaderPeek.h"
#include "ResourceManagement/Shader.h"
#include <imgui.h>
#include <imgui_stdlib.h>

void ShaderPeek::GetProperties()
{
	Properties.clear();

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

		Properties.push_back(Property(name, type));
		auto* p = &Properties.back();
		switch (type)
		{
		case GL_FLOAT:
			p->data = new std::byte[sizeof(float)];
			break;
		case GL_FLOAT_VEC4:
			p->data = new std::byte[sizeof(glm::vec4)];
			break;
		}
	}
}

void ShaderPeek::Inspect()
{

	ImGui::Text("Material");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 1, 1), "Shader = LoadModel");

	Shader->Use();

	for (auto& p : Properties)
	{
		switch (p.type)
		{
		case GL_FLOAT:
			if (ImGui::InputFloat(p.name.c_str(), (float*)p.data))
			{
				auto t = *(float*)p.data;
				Shader->SetFloat(p.name.c_str(), t);
			}
			break;
		case GL_FLOAT_VEC4:
			if (ImGui::ColorEdit4(p.name.c_str(), (float*)p.data, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_Float))
			{
				auto* t = (float*)p.data;
				Shader->SetVector4f(p.name.c_str(), glm::vec4(t[0], t[1], t[2], t[3]));
			}
			break;
		}
	}
}
