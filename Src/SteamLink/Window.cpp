// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <SDL2/SDL_syswm.h>

#include "Defines.h"
#include "KeyDefines.h"
#include "Log.h"
#include "System.h"
#include "Window.h"

Window::Window()	:
	m_pSDLWindow(NULL),
	m_displayType(0),
	m_windowType(0)
{
}

Window::~Window()
{
	close();
}

Window::Ptr Window::create()
{
	INSTANCE(pWindow, Window())

	if (false == pWindow->initialize())
	{
		pWindow.reset();
	}

	return	pWindow;
}

bool Window::initialize()
{
	if (false == createWindow())
	{
		return	false;
	}

	if (NULL == m_pSDLWindow)
	{
		return	false;
	}

	SDL_ShowWindow(m_pSDLWindow);

	return	true;
}

void Window::close()
{
	if (m_pSDLWindow != NULL)
	{
		SDL_DestroyWindow(m_pSDLWindow);

		m_pSDLWindow	= NULL;
	}
}

bool Window::update()
{
	SDL_Event	event;

	while (1 == SDL_PollEvent(&event))
	{
		Log::instance()->logError("Event type %d", event.type);

		switch (event.type)
		{
			case SDL_CONTROLLERDEVICEADDED:
            	SDL_GameControllerOpen(event.cdevice.which);
            	
				break;

			case SDL_QUIT:
				System::setExit(true);
	
				Log::instance()->logError("Exiting");
				Log::reset();

				break;

				break;

			case SDL_CONTROLLERBUTTONDOWN:
				System::setExit(true);
	
				Log::instance()->logError("Exiting");
				Log::reset();

				break;

			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
						Log::instance()->logError("Window event close");
						Log::reset();
		
						break;
		
					default:
						break;
				}

				break;
		
			default:
				break;
		}
	}

	return	true;
}

void Window::quit()
{
}

bool Window::createWindow()
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		Log::instance()->logError("Unable to initialize SDL: %s", SDL_GetError());

		return	false;
	}

	SDL_DisplayMode	displayMode;

	if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
	{
		Log::instance()->logError("Unable to get desktop display mode: %s", SDL_GetError());

		return	false;
	}

	m_iWidth	= displayMode.w;
	m_iHeight	= displayMode.h;

	m_pSDLWindow	= SDL_CreateWindow(gsc_szWindowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_iWidth, m_iHeight, 
		SDL_WINDOW_FULLSCREEN | SDL_WINDOW_HIDDEN);

	if (NULL == m_pSDLWindow)
	{
		Log::instance()->logError("Unable to create SDL window: %s", SDL_GetError());

		return	false;
	}

	SDL_SysWMinfo	sysWMinfo;

	SDL_VERSION(&sysWMinfo.version);

	if (SDL_GetWindowWMInfo(m_pSDLWindow, &sysWMinfo) < 0)
	{
		Log::instance()->logError("Unable to get window info: %s", SDL_GetError());

		return	false;
	}

	if (sysWMinfo.subsystem != SDL_SYSWM_VIVANTE)
	{
		Log::instance()->logError("Windows subsystem info is incorrect");

		return	false;
	}
	
	m_displayType	= sysWMinfo.info.vivante.display;
	m_windowType	= sysWMinfo.info.vivante.window;

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER))
	{
		Log::instance()->logError("Unable to initialize SDL game controler system.");

		return	false;
	}

	return	true;
}