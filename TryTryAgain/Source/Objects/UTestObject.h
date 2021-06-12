#pragma once
#include "UObject.h"

NOTE(UTestObject, Super<UObject>)
class UTestObject : public UObject
{
public:
	float f3;
	REFLECT_NOTED(UTestObject, f3)
};

