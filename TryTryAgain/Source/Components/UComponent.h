#pragma once

#include "Objects/UObject.h"

class AActor;

class UComponent : public UObject
{

public:

	UComponent() {}

	UComponent(AActor* owner);

	bool isActive = true;

	virtual void Init();
	
	virtual void Tick(float delta);

protected:
	AActor* owner;
};
