#include <atomic>
#include <signal.h>

#include "Functions.h"
#include "SegFaultHandler.h"
#include "System.h"

int main(int _argc, char** _argv)
{
	int	iRetVal	= 0;

	setSegFaultHandler();

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

	fflush(stderr);

	resetSegFaultHandler();

	return	iRetVal;
}