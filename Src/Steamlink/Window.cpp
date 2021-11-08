// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Defines.h"
#include "KeyDefines.h"
#include "Log.h"
#include "System.h"
#include "Window.h"

int Window::ms_iScreenWidth		= 0;	// Screen size
int Window::ms_iScreenHeight	= 0;

Window::Window()	:
	m_pInputContext(NULL)
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
	m_iWidth	= ms_iScreenWidth;
	m_iHeight	= ms_iScreenHeight;

	if (false == createInputContext())
	{
		return	false;
	}

	return	true;
}

void Window::close()
{
	if (m_pInputContext != NULL)
	{
		releaseInputContext();

		m_pInputContext	= NULL;
	}
}

bool Window::update()
{
	if (m_pInputContext != NULL)
	{
		if (1 == RokuInputContext_hasEvents(m_pInputContext))
		{
			RokuInputEvent*	pEvent	= NULL;

			while (1 == RokuInputContext_getEvent(m_pInputContext, &pEvent))
			{
				switch (RokuInputEvent_getType(pEvent))
				{
					case ROKU_INPUT_EVENT_TYPE_BUTTON_RELEASED:
					{
						std::lock_guard<std::mutex>	lock(m_inputMutex);

						int	iKey	= -1;

						switch (RokuInputEvent_getButton(pEvent))
						{
							case 0x01:
								iKey	= InputLeft;

								break;

							case 0x02:
								iKey	= InputRight;

								break;

							case 0x04:
								iKey	= InputDown;

								break;

							case 0x08:
								iKey	= InputUp;

								break;

							case 0x20:
								iKey	= InputButton2;

								break;

							case 0x40:
								iKey	= InputButton3;

								break;

							case 0x80:
								iKey	= InputButton1;

								break;
						}

						if (iKey != -1)
						{
							System::keyUp(iKey);
						}
			
						break;
					}
				
					case ROKU_INPUT_EVENT_TYPE_BUTTON_PRESSED:
					{
						std::lock_guard<std::mutex>	lock(m_inputMutex);

						int	iKey	= -1;

						switch (RokuInputEvent_getButton(pEvent))
						{
							case 0x01:
								iKey	= InputLeft;

								break;

							case 0x02:
								iKey	= InputRight;

								break;

							case 0x04:
								iKey	= InputDown;

								break;

							case 0x08:
								iKey	= InputUp;

								break;

							case 0x20:
								iKey	= InputButton2;

								break;

							case 0x40:
								iKey	= InputButton3;

								break;

							case 0x80:
								iKey	= InputButton1;

								break;
						}

						if (iKey != -1)
						{
							System::keyDown(iKey);
						}
			
						break;
					}
			
#if 0
					case ROKU_INPUT_EVENT_TYPE_POINTER:
					{
						std::lock_guard<std::mutex>	lock(m_inputMutex);

						System::mouseMove(RokuInputEvent_getPointerX(pEvent), RokuInputEvent_getPointerY(pEvent));

						break;
					}

					case ROKU_INPUT_EVENT_TYPE_MOTION:
						break;

					case ROKU_INPUT_EVENT_TYPE_KEY_RELEASED:
					{
						std::lock_guard<std::mutex>	lock(m_inputMutex);

						RokuInputEvent_getKey(pEvent);

						break;
					}
					
					case ROKU_INPUT_EVENT_TYPE_KEY_PRESSED:
					{
						std::lock_guard<std::mutex>	lock(m_inputMutex);

						RokuInputEvent_getKey(pEvent);
		
						break;
					}
#endif
				
					default:
						break;
				}
			}

			RokuInputEvent_release(pEvent);
		}
	}

	return	true;
}

void Window::quit()
{
}

bool Window::createInputContext()
{
	if (RokuInputContext_create(&m_pInputContext) != 0)
	{
		Log::instance()->logError("Unable to create Roku input context");

		return	false;
	}

	return	true;
}

void Window::releaseInputContext()
{
	RokuInputContext_release(m_pInputContext);
}
