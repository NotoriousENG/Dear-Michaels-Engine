#pragma once

char TempBuffer[255];

struct FString
{
	FString(const char* fmt, ...)
	{
        va_list args;
        va_start(args, fmt);
        const char* label_end = TempBuffer + vsnprintf(TempBuffer, ((int)(sizeof(TempBuffer) / sizeof(*(TempBuffer)))), fmt, args);
        va_end(args);

		const char* label = TempBuffer;

		char* it = Text;

		while(label <= label_end)
		{
			*it = *label;

			label++;
			it++;
		}
	}
	char Text[255];
};


