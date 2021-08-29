#pragma once
#include <string>

enum class R_TYPE
{
	UNSPECIFIED,
	TEXTURE,
	SHADER,
	MODEL,
	MATERIAL,
	COMPOSITE
};
class Resource
{
private:
	std::string GUID;
	R_TYPE type = R_TYPE::UNSPECIFIED;

};

