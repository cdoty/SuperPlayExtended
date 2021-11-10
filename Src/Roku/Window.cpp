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
				if (NULL == pEvent)
				{
					break;
				}

				int32_t	iEventType	= RokuInputEvent_getType(pEvent);

				switch (iEventType)
				{
					case ROKU_INPUT_EVENT_TYPE_BUTTON_RELEASED:
					{
						std::lock_guard<std::mutex>	lock(m_inputMutex);

						Inputs	key	= InputNone;

						switch (RokuInputEvent_getButton(pEvent))
						{
							case ROKU_INPUT_BUTTON_UP:
								key	= InputLeft;

								break;

							case ROKU_INPUT_BUTTON_DOWN:
								key	= InputRight;

								break;

							case ROKU_INPUT_BUTTON_LEFT:
								key	= InputDown;

								break;

							case ROKU_INPUT_BUTTON_RIGHT:
								key	= InputUp;

								break;

							case ROKU_INPUT_BUTTON_FWD:
							case ROKU_INPUT_BUTTON_A:
								key	= InputButton1;

								break;
							case ROKU_INPUT_BUTTON_PLAY:
							case ROKU_INPUT_BUTTON_B:
								key	= InputButton2;

								break;

							case ROKU_INPUT_BUTTON_RWD:
							case ROKU_INPUT_BUTTON_SELECT:
								key	= InputButtonStart;

								break;

							case ROKU_INPUT_BUTTON_BACK:
							case ROKU_INPUT_BUTTON_EXIT:
								key	= InputButtonExit;

								break;
						}

						if (key != InputNone)
						{
							System::keyUp(key);
						}

						break;
					}
				
					case ROKU_INPUT_EVENT_TYPE_BUTTON_PRESSED:
					{
						std::lock_guard<std::mutex>	lock(m_inputMutex);
				
						Inputs	key	= InputNone;

						switch (RokuInputEvent_getButton(pEvent))
						{
							case ROKU_INPUT_BUTTON_UP:
								key	= InputLeft;

								break;

							case ROKU_INPUT_BUTTON_DOWN:
								key	= InputRight;

								break;

							case ROKU_INPUT_BUTTON_LEFT:
								key	= InputDown;

								break;

							case ROKU_INPUT_BUTTON_RIGHT:
								key	= InputUp;

								break;

							case ROKU_INPUT_BUTTON_FWD:
							case ROKU_INPUT_BUTTON_A:
								key	= InputButton1;

								break;
							case ROKU_INPUT_BUTTON_PLAY:
							case ROKU_INPUT_BUTTON_B:
								key	= InputButton2;

								break;

							case ROKU_INPUT_BUTTON_RWD:
							case ROKU_INPUT_BUTTON_SELECT:
								key	= InputButtonStart;

								break;

							case ROKU_INPUT_BUTTON_BACK:
							case ROKU_INPUT_BUTTON_EXIT:
								key	= InputButtonExit;

								break;
						}
				
						if (key != InputNone)
						{
							System::keyDown(key);
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

				RokuInputEvent_release(pEvent);
			}
		}
	}

	Log::instance()->logError("Function exit");
				
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
