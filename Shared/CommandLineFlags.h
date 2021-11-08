#pragma once

#include <stdint.h>

extern	uint32_t	gs_commandLineFlags;	// Command line flags

enum CommandLineFlags
{
	TestModeFlag			= 0x00000001,
};

inline void setFlags(CommandLineFlags _eCommandLineFlags)
{
	gs_commandLineFlags	|= _eCommandLineFlags;
}

inline void clearFlags(CommandLineFlags _eCommandLineFlags)
{
	gs_commandLineFlags	&= ~_eCommandLineFlags;
}

inline bool isTestMode()
{
	return	TestModeFlag == (gs_commandLineFlags & TestModeFlag);
};
