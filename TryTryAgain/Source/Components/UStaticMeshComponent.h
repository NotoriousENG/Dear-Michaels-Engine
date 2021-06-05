#pragma once
#include "UComponent.h"
class UStaticMeshComponent : public UComponent
{
public:

	UStaticMeshComponent(AActor* owner);

	void Tick(float delta) override;
};

