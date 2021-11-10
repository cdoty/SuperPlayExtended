// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <SDL2/SDL_syswm.h>

#include "Defines.h"
#include "KeyDefines.h"
#include "Log.h"
#include "System.h"
#include "Window.h"

static const int gsc_iDeadZone	= 64;	// Controller dead zone

Window::Window()	:
	m_pSDLWindow(NULL),
	m_displayType(0),
	m_windowType(0),
	m_controllerStatus(0)
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
            	SDL_GameControllerOpen(event.jdevice.which);
            	
				break;

			case SDL_QUIT:
				System::setExit(true);
	
				break;

			case SDL_CONTROLLERBUTTONDOWN:
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

				break;
			}

			case SDL_CONTROLLERBUTTONUP:
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

				break;
			}

			case SDL_CONTROLLERAXISMOTION:
			{
				int	iValue	= event.caxis.value;

				switch (event.caxis.axis)
				{
					case SDL_CONTROLLER_AXIS_LEFTX:
						if (iValue < -gsc_iDeadZone)
						{
							if (m_controllerStatus & InputRight)
							{
								m_controllerStatus	&= ~ InputRight;

								System::keyUp(InputRight);
							}

							m_controllerStatus	|= InputLeft;

							System::keyDown(InputLeft);
						}

						else if (iValue > gsc_iDeadZone)
						{
							if (m_controllerStatus & InputLeft)
							{
								m_controllerStatus	&= ~ InputLeft;

								System::keyUp(InputLeft);
							}

							m_controllerStatus	|= InputRight;

							System::keyDown(InputRight);
						}

						else
						{
							if (m_controllerStatus & InputLeft)
							{
								m_controllerStatus	&= ~ InputLeft;

								System::keyUp(InputLeft);
							}

							if (m_controllerStatus & InputRight)
							{
								m_controllerStatus	&= ~ InputRight;

								System::keyUp(InputRight);
							}
						}
						
						break;

					case SDL_CONTROLLER_AXIS_LEFTY:
						if (iValue < -gsc_iDeadZone)
						{
							if (m_controllerStatus & InputDown)
							{
								m_controllerStatus	&= ~ InputDown;

								System::keyUp(InputDown);
							}

							m_controllerStatus	|= InputUp;

							System::keyDown(InputUp);
						}

						else if (iValue > gsc_iDeadZone)
						{
							if (m_controllerStatus & InputUp)
							{
								m_controllerStatus	&= ~ InputUp;

								System::keyUp(InputUp);
							}

							m_controllerStatus	|= InputDown;

							System::keyDown(InputDown);
						}

						else
						{
							if (m_controllerStatus & InputUp)
							{
								m_controllerStatus	&= ~ InputUp;

								System::keyUp(InputUp);
							}

							if (m_controllerStatus & InputDown)
							{
								m_controllerStatus	&= ~ InputDown;

								System::keyUp(InputDown);
							}
						}

						break;
				
					default:
						break;
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