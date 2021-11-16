#include <atomic>
#include <signal.h>

#include "Functions.h"
#include "SegFaultHandler.h"
#include "System.h"

void handler(int _signal, siginfo_t* /*_signalInfo*/, void* /*_pParameter*/)
{
	switch (_signal)
	{
		case SIGTERM:
			System::setExit(true);
			
			break;
		
		default:
			break;
	}
}

int main(int _argc, char** _argv)
{
	if (3 == _argc)
	{
		SystemWindow::ms_iScreenWidth		= Functions::convertStringToInt(_argv[1]);
		SystemWindow::ms_iScreenHeight	= Functions::convertStringToInt(_argv[2]);
	}

	int	iRetVal	= 0;

	setSegFaultHandler();

	struct sigaction	oldHandler;
	struct sigaction	newHandler;

    newHandler.sa_sigaction	= handler;
    sigemptyset(&newHandler.sa_mask);

    newHandler.sa_flags	= SA_SIGINFO | SA_RESTART;

	sigaction(SIGTERM, &newHandler, &oldHandler);

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

	sigaction(SIGTERM, &oldHandler, NULL);

	fflush(stderr);

	resetSegFaultHandler();

	return	iRetVal;
}