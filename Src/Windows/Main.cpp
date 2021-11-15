// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CommandLine.h"
#include "Defines.h"
#include "SegFaultHandler.h"
#include "System.h"

uint32_t	gs_commandLineFlags	= 0;	// Command line flags

ENABLEGPU;

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _iCmdShow)
{
	int	iRetVal	= 0;

	setSegFaultHandler();

	std::string	strCmdLine	= _lpCmdLine;

	processCommandLine(strCmdLine);

	if (false == System::initialize())
	{
		iRetVal	= 1;
	}

	else
	{
		while (false == System::getExit())
		{
			System::run();
		}
	}

	System::close();

	resetSegFaultHandler();

	return	iRetVal;
}