#pragma once

#include<stdarg.h>
#include<stdio.h>

struct FString
{
	// Constructs FString with accessable Text Element (char [255])
	FString(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		vsnprintf(Text, ((int)(sizeof(Text) / sizeof(*(Text)))), fmt, args);
		va_end(args);
	}
	char Text[255];
};
