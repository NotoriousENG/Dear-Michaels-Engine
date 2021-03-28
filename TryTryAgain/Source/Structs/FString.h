#pragma once

char TempBuffer[3073];

struct FString
{
	FString(const char* fmt, ...)
	{
        va_list args;
        va_start(args, fmt);
        const char* label_end = TempBuffer + vsnprintf(TempBuffer, IM_ARRAYSIZE(TempBuffer), fmt, args);
        va_end(args);

		const char* label = TempBuffer;

		Text = new char[(unsigned long) label_end - (unsigned long) label];
		char* it = Text;

		while(label <= label_end)
		{
			*it = *label;

			label++;
			it++;
		}
	}
	char* Text;
};


