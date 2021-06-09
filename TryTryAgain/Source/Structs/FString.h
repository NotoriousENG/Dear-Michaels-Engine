#pragma once

struct FString
{
	FString(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		vsnprintf(Text, ((int)(sizeof(Text) / sizeof(*(Text)))), fmt, args);
		va_end(args);
	}
	char Text[255];
};
