#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#endif

void debugOutput(const char* _szOutput)
{
#if defined _DEBUG
	OutputDebugStringA(_szOutput);
	OutputDebugStringA("\n");
#endif
}
