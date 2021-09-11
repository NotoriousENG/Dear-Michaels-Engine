#pragma once

#include <string>

namespace rm
{
	class Resource
	{
	public:
		virtual ~Resource() {};
		virtual void Init(std::string path) {};
	};
}


