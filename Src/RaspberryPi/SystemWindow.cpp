// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include  <X11/Xatom.h>
#include  <X11/Xutil.h>

#include "Defines.h"
#include "KeyDefines.h"
#include "Log.h"
#include "System.h"
#include "SystemWindow.h"

static const int gsc_iDeadZone	= 4096;	// Controller dead zone

SystemWindow::SystemWindow()	:
	m_pDisplay(NULL),
	m_iXDirection(0),
	m_iYDirection(0)
{
}

SystemWindow::~SystemWindow()
{
	close();
}

SystemWindow::Ptr SystemWindow::create()
{
	INSTANCE(pSystemWindow, SystemWindow())

	if (false == pSystemWindow->initialize())
	{
		pSystemWindow.reset();
	}

	return	pSystemWindow;
}

bool SystemWindow::initialize()
{
	if (false == createWindow())
	{
		return	false;
	}

	return	true;
}

void SystemWindow::close()
{
}

bool SystemWindow::update()
{
#if 0
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
	
				break;

			case SDL_CONTROLLERBUTTONDOWN:
			{
				if (0 == event.cdevice.which)
				{
					Inputs	key	= InputNone;
					
					switch (event.cbutton.button)
					{
						case SDL_CONTROLLER_BUTTON_DPAD_UP:
							key	= InputUp;

							break;

						case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
							key	= InputDown;
							
							break;
							
						case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
							key	= InputLeft;
							
							break;
							
						case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
							key	= InputRight;
							
							break;
							
						case SDL_CONTROLLER_BUTTON_A:
							key	= InputButton1;
							
							break;

						case SDL_CONTROLLER_BUTTON_B:
							key	= InputButton2;
							
							break;

						case SDL_CONTROLLER_BUTTON_START:
							key	= InputButtonStart;
							
							break;

						case SDL_CONTROLLER_BUTTON_BACK:
							key	= InputButtonExit;
							
							break;
					
						default:
							break;
					}
					
					if (key != InputNone)
					{
						System::keyDown(key);
					}
				}

				break;
			}

			case SDL_CONTROLLERBUTTONUP:
			{
				if (0 == event.cdevice.which)
				{
					Inputs	key	= InputNone;
					
					switch (event.cbutton.button)
					{
						case SDL_CONTROLLER_BUTTON_DPAD_UP:
							key	= InputUp;

							break;

						case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
							key	= InputDown;
							
							break;
							
						case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
							key	= InputLeft;
							
							break;
							
						case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
							key	= InputRight;
							
							break;
							
						case SDL_CONTROLLER_BUTTON_A:
							key	= InputButton1;
							
							break;

						case SDL_CONTROLLER_BUTTON_B:
							key	= InputButton2;
							
							break;

						case SDL_CONTROLLER_BUTTON_START:
							key	= InputButtonStart;
							
							break;

						case SDL_CONTROLLER_BUTTON_BACK:
							key	= InputButtonExit;
							
							break;
					
						default:
							break;
					}
					
					if (key != InputNone)
					{
						System::keyUp(key);
					}
				}

				break;
			}

			case SDL_CONTROLLERAXISMOTION:
			{
				if (0 == event.cdevice.which)
				{
					int	iValue	= event.caxis.value;

					switch (event.caxis.axis)
					{
						case SDL_CONTROLLER_AXIS_LEFTX:
						{
							int	iNewDirection	= 0;

							if (iValue < -gsc_iDeadZone)
							{
								iNewDirection	= -1;
							}

							else if (iValue > gsc_iDeadZone)
							{
								iNewDirection	= 1;
							}
							
							if (m_iXDirection != iNewDirection && m_iXDirection != 0)
							{
								if (-1 == m_iXDirection)
								{
									System::keyUp(InputLeft);
								}

								else
								{
									System::keyUp(InputRight);
								}
							}

							if (iNewDirection != 0)
							{
								if (-1 == iNewDirection)
								{
									System::keyDown(InputLeft);
								}

								else
								{
									System::keyDown(InputRight);
								}
							}

							m_iXDirection	= iNewDirection;

							break;
						}

						case SDL_CONTROLLER_AXIS_LEFTY:
						{
							int	iNewDirection	= 0;

							if (iValue < -gsc_iDeadZone)
							{
								iNewDirection	= -1;
							}

							else if (iValue > gsc_iDeadZone)
							{
								iNewDirection	= 1;
							}

							if (m_iYDirection != iNewDirection && m_iYDirection != 0)
							{
								if (-1 == m_iYDirection)
								{
									System::keyUp(InputUp);
								}

								else
								{
									System::keyUp(InputDown);
								}
							}

							if (iNewDirection != 0)
							{
								if (-1 == iNewDirection)
								{
									System::keyDown(InputUp);
								}

								else
								{
									System::keyDown(InputDown);
								}
							}

							m_iYDirection	= iNewDirection;

							break;
						}

						default:
							break;
					}
				}
				
				break;
			}

			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
						break;
		
					default:
						break;
				}

				break;
		
			default:
				break;
		}
	}
#endif

	return	true;
}

void SystemWindow::quit()
{
}

bool SystemWindow::createWindow()
{
#if 0
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

	m_pSDLWindow	= SDL_CreateWindow(gsc_szWindowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_iWidth, m_iHeight, 0);

	if (NULL == m_pSDLWindow)
	{
		Log::instance()->logError("Unable to create SDL window: %s", SDL_GetError());

		return	false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER))
	{
		Log::instance()->logError("Unable to initialize SDL game controler system.");

		return	false;
	}
#endif

	m_pDisplay	= XOpenDisplay(NULL);

	if (NULL == m_pDisplay)
	{
		Log::instance()->logError("Unable to initialize X display connection");

		return	false;
	}

	Screen*	pScreen	= DefaultScreenOfDisplay(m_pDisplay);

	m_iWidth	= pScreen->width;
	m_iHeight	= pScreen->height;

	Window rootWindow	=  DefaultRootWindow(m_pDisplay);

	XSetWindowAttributes	windowAttributes;

	windowAttributes.event_mask	= ExposureMask | PointerMotionMask | KeyPressMask;

	m_window	= XCreateWindow(m_pDisplay, rootWindow, 0, 0, m_iWidth, m_iHeight);
	
	return	true;
}
