#pragma once

#include <signal.h>

#include "Log.h"

typedef void (*SignalHandlerPointer)(int);

// Signal handler
static void signalHandler(int _signal)
{
	Log::instance()->logError("Caught exception %d", _signal);
	Log::reset();

	exit(0);
}

static SignalHandlerPointer	previousHandler;

void setSegFaultHandler()
{
	previousHandler	= signal(SIGSEGV, signalHandler);
}

void resetSegFaultHandler()
{
	signal(SIGSEGV, previousHandler);
}
