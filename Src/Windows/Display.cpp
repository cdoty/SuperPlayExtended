// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Defines.h"
#include "Display.h"
#include "Log.h"
#include "System.h"

#pragma comment(lib, "libEGL.dll.lib")
#pragma comment(lib, "libGLESv2.dll.lib")

Display::Display()	:
	super()
{
}

Display::Ptr Display::create()
{
	INSTANCE(pDisplay, Display())

	if (false == pDisplay->initialize())
	{
		pDisplay.reset();
	}

	return	pDisplay;
}

bool Display::initializeEGL()
{
	m_eglDisplay	= eglGetDisplay(EGL_DEFAULT_DISPLAY);

	if (EGL_NO_DISPLAY == m_eglDisplay)
	{
		Log::instance()->logError("Unable to get EGL display");

		return	false;
	}

	EGLint	majorVersion;
	EGLint	minorVersion;

	if (EGL_FALSE == eglInitialize(m_eglDisplay, &majorVersion, &minorVersion))
	{
		Log::instance()->logError("Unable to initialize EGL");

		return	false;
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	const EGLint configAttributes[]	=
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE,24,
		EGL_NONE
	};

	EGLint	configCount	= 0;

	if (EGL_FALSE == eglChooseConfig(m_eglDisplay, configAttributes, &m_eglConfig, 1, &configCount))
	{
		Log::instance()->logError("Unable to choose EGL config %04X", eglGetError());

		return	false;
	}

	HWND	hWnd	= NULL;

	Window::Ptr	pWindow	= System::getWindow();

	if (pWindow != nullptr)
	{
		hWnd	= pWindow->getHwnd();
	}

	if (NULL == hWnd)
	{
		Log::instance()->logError("Window handle is invalid");

		return	false;
	}

	m_eglSurface	= eglCreateWindowSurface(m_eglDisplay, m_eglConfig, (EGLNativeWindowType)hWnd, NULL);

	if (EGL_NO_SURFACE == m_eglSurface)
	{
		Log::instance()->logError("Unable to create EGL window surface %04X", eglGetError());

		return	false;
	}

	EGLint	contextAttributes[]	=
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	m_eglContext	= eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, contextAttributes);

	if (EGL_NO_CONTEXT == m_eglContext)
	{
		Log::instance()->logError("Unable to create EGL context %04X", eglGetError());

		return	false;
	}

	if (EGL_FALSE == eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext))
	{
		Log::instance()->logError("Unable to set EGL context %04X", eglGetError());

		return	false;
	}

	if (EGL_FALSE == eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_WIDTH, &m_iWidth))
	{
		Log::instance()->logError("Unable to get surface width %04X", eglGetError());

		return	false;
	}

	if (EGL_FALSE == eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_HEIGHT, &m_iHeight))
	{
		Log::instance()->logError("Unable to get surface height %04X", eglGetError());

		return	false;
	}

	return	true;
}
