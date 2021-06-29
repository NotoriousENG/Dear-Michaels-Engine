#pragma once
#include "UTestObject.h"
#include <ThirdParty/RareCpp/Reflect.h>

NOTE(UTestTestObject, Super<UTestObject>)
class UTestTestObject : public UTestObject
{
public:

	float f4;

	virtual void Serialize() override {
		Print<UTestTestObject>(*this);
	};

	REFLECT_NOTED(UTestTestObject, f4)
};

