#pragma once
#include "UComponent.h"
#include <memory>

namespace rm {
    class Shader;
	class Model;
}

using namespace std;

class UStaticModelComponent : public UComponent
{

public:

	rm::Model* Model;
	rm::Shader* Shader;

	UStaticModelComponent(AActor* owner);

	void Draw(rm::Shader* shader = nullptr);

	bool bDraw = true;

private:
};
