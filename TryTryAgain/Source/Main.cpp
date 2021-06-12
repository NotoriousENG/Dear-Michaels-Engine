#include "Main.h"

#include <memory>
#include <SDL2/SDL.h>

#include "Application.h"
#include "Objects/UObject.h"
#include "Objects/UTestObject.h"

#include <iostream>
#include <typeinfo>
#include <Json.h>

using Reflect::is_reflected;
using Json::Statics;

#undef main

int main(void)
{
	//// Initialize Application
	//auto app = std::make_unique<Application>("Michael's App");

	//// Start Application loop
	//app->Loop();

	std::vector<std::unique_ptr<UObject>> objects;

	UObject test;
	test.f = 0;
	test.f2 = 1;

	objects.push_back(std::make_unique<UObject>(test));

	UTestObject test2;
	test2.f = 0;
	test2.f2 = 1;
	test2.f3 = 4;

	objects.push_back(std::make_unique<UTestObject>(test2));

	for (auto& o : objects)
	{
		o->Serialize();
	}

	

	

	return 0;
}
