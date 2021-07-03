#pragma once
#include "UObject.h"

NOTE(UTestObject, Super<UObject>)
class UTestObject : public UObject
{
	typedef UObject Super;

public:

	static std::unique_ptr<UObject> create() { return std::make_unique<UTestObject>(); }

	float f3;

	virtual void Serialize() override{
		Print<UTestObject>(*this);
	};

	REFLECT_NOTED(UTestObject, f3)
};

