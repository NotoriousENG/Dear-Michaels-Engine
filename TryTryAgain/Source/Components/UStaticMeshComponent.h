#pragma once
#include "UComponent.h"
#include <memory>

namespace rm {
	class Mesh;
	class Shader;
}


class UStaticMeshComponent : public UComponent
{

public:

	rm::Mesh* Mesh;
	rm::Shader* Shader;

	UStaticMeshComponent() {}

	UStaticMeshComponent(AActor* owner);

	void Tick(float delta) override;

	void Draw(rm::Shader* shader = nullptr);

	bool bDraw = true;
};
