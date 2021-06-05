#pragma once

class AActor;

class UComponent
{

public:

	UComponent(AActor* owner);

	bool isActive = true;

	virtual void Init();
	
	virtual void Tick(float delta);

protected:

	AActor* owner;
};

