#pragma once

#include "Objects/UObject.h"

class AActor;

class UComponent : public UObject
{

public:

	UComponent() {}

	UComponent(std::shared_ptr<AActor> owner);

	bool isActive = true;

	virtual void Init();
	
	virtual void Tick(float delta);

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar( CEREAL_NVP(owner), CEREAL_NVP(isActive));
	}

	AActor* GetOwner()
	{
		return owner.get();
	}

protected:
	std::shared_ptr<AActor> owner;

	friend class AActor;
};

CEREAL_REGISTER_TYPE(UComponent);