#pragma once

#include <string>
#include <vector>

namespace rm
{
	class Resource
	{
	public:
		virtual ~Resource() {};
		virtual void Init(std::string path) {};

		std::string path;

		virtual std::vector<std::string> ValidExtensions() { return std::vector<std::string>(); }
	};
}


