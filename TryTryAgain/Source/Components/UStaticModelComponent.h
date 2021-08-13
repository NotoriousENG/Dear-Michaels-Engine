#pragma once
#include "UComponent.h"
#include <memory>
#include <string>

namespace rm {
    class Shader;
	class Model;
}

using namespace std;

class UStaticModelComponent : public UComponent
{

	enum class SHADER {
		FRAG = 0,
		VERT = 1,
		GEOM = 2,
		SHORT_NAME = 3
	};

public:

	std::shared_ptr<rm::Model> Model;
	std::shared_ptr<rm::Shader> Shader;

	UStaticModelComponent(std::shared_ptr<AActor> owner);

	UStaticModelComponent() {}

	virtual void Init() override;

	void Draw(rm::Shader* shader = nullptr);

	bool bDraw = true;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::make_nvp("UComponent", cereal::virtual_base_class<UComponent>(this)), CEREAL_NVP(ShaderPaths), CEREAL_NVP(ModelPath));
	}

	std::string ModelPath = "Assets/Models/mario/mario-tex.obj";

private:
	std::string ShaderPaths[4] = { "Assets/Shaders/LoadModel.vert", "Assets/Shaders/LoadModel.frag", "", "LoadModel" };
};

CEREAL_REGISTER_TYPE(UStaticModelComponent);