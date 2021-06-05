#pragma once
#include "UComponent.h"
#include <memory>

class UStaticMeshComponent : public UComponent
{

public:

	UStaticMeshComponent(AActor* owner);

	void Tick(float delta) override;

	void Draw();
};

