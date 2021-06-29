#include "Main.h"

#include <memory>
#include <SDL2/SDL.h>

#include "Application.h"
#include "Objects/UObject.h"
#include "Objects/UTestObject.h"

#include <iostream>
#include <typeinfo>
#include <Json.h>
#include <Objects/UTestTestObject.h>

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
	test2.f = 2;
	test2.f2 = 3;
	test2.f3 = 4;

	objects.push_back(std::make_unique<UTestObject>(test2));

	UTestTestObject test3;
	test3.f = 5;
	test3.f2 = 6;
	test3.f3 = 7;
	test3.f4 = 8;

	objects.push_back(std::make_unique<UTestTestObject>(test3));

	for (auto& o : objects)
	{
		o->Serialize();
	}

	//UTestTestObject test4;
	//std::stringstream s;
	//s << p;
	//s >> Json::in(test4);
	//std::cout << Json::pretty(test4) << std::endl;

	return 0;
}
