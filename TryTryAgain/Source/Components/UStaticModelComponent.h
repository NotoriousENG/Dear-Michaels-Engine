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

public:

	std::shared_ptr<rm::Model> Model;
	std::shared_ptr<rm::Shader> Shader;

	UStaticModelComponent(std::shared_ptr<AActor> owner);

	UStaticModelComponent() {}

	virtual void Init() override;

	void Draw(rm::Shader* shader = nullptr);

	bool bDraw = true;

};
