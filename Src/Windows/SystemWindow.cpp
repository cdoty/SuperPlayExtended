// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <Windowsx.h>

#include "CommandLineFlags.h"
#include "Defines.h"
#include "Functions.h"
#include "KeyDefines.h"
#include "Log.h"
#include "System.h"
#include "SystemWindow.h"
#include "..\Resources\Windows\Resource.h"

SystemWindow::SystemWindow()	:
	super(),
	m_hInstance(NULL),
	m_hWnd(NULL),
	m_classAtom(INVALID_ATOM),
	m_rctMonitor({}),
	m_rctWindow({})
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
	if (true == isTestMode())
	{
		m_iWidth	= gsc_iTestWidth;
		m_iHeight	= gsc_iTestHeight;

		m_rctMonitor.left	= (GetSystemMetrics(SM_CXSCREEN) - m_iWidth) / 2;
		m_rctMonitor.top	= (GetSystemMetrics(SM_CYSCREEN) - m_iHeight) / 2;
		m_rctMonitor.right	= m_rctMonitor.left + m_iWidth;
		m_rctMonitor.bottom	= m_rctMonitor.top + m_iHeight;
	}

	else
	{
		m_iWidth	= GetSystemMetrics(SM_CXSCREEN);
		m_iHeight	= GetSystemMetrics(SM_CYSCREEN);

		m_rctMonitor.left	= 0;
		m_rctMonitor.top	= 0;
		m_rctMonitor.right	= m_iWidth;
		m_rctMonitor.bottom	= m_iHeight;
	}

	m_rctWindow.left	= 0;
	m_rctWindow.top		= 0;
	m_rctWindow.right	= m_rctMonitor.right;
	m_rctWindow.bottom	= m_rctMonitor.bottom;

	m_hInstance	= GetModuleHandle(NULL);
	
	WNDCLASSA	wndClass;	
	
	wndClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc	= messageProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= m_hInstance;
	wndClass.hIcon			= LoadIcon(m_hInstance, MAKEINTRESOURCE(ICON_APPLICATION));
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground	= NULL;
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= gsc_szAppName;

	// Register windows class
	m_classAtom	= RegisterClassA(&wndClass);

	if (0 == m_classAtom)
	{
		Log::instance()->logError("Unable to register windows class (%08X)", GetLastError());
		
		return	false;
	}
	
	// Create window
#ifdef _DEBUG
	m_hWnd	= CreateWindowExA(WS_EX_TOOLWINDOW, gsc_szAppName, gsc_szWindowTitle, WS_POPUP, m_rctMonitor.left, m_rctMonitor.top, 
		m_rctMonitor.right - m_rctMonitor.left, m_rctMonitor.bottom - m_rctMonitor.top, NULL, NULL, m_hInstance, this);
#else
	m_hWnd	= CreateWindowExA(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, gsc_szAppName, gsc_szWindowTitle, WS_POPUP, m_rctWindow.left, m_rctWindow.top, 
		m_rctMonitor.right - m_rctMonitor.left, m_rctMonitor.bottom - m_rctMonitor.top, NULL, NULL, m_hInstance, this);
#endif
	
	if (NULL == m_hWnd)
	{
		Log::instance()->logError("Unable to create window (%08X)", GetLastError());
		
		return	false;
	}

	// Show and activate window
	ShowWindow(m_hWnd, SW_SHOW);
	SetActiveWindow(m_hWnd);
	SetFocus(m_hWnd);
	
	return	true;
}

void SystemWindow::close()
{
}

bool SystemWindow::update()
{
	MSG	msg;

	memset(&msg, 0, sizeof(MSG));

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{ 
		if (WM_QUIT == msg.message)
		{
			return	false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return	true;
}

void SystemWindow::quit()
{
	PostQuitMessage(0);
}

void SystemWindow::setMonitorRect(const RECT& _rctMonitor)
{
	m_rctMonitor.left	= _rctMonitor.left;
	m_rctMonitor.top	= _rctMonitor.top;
	m_rctMonitor.right	= _rctMonitor.right;
	m_rctMonitor.bottom	= _rctMonitor.bottom;
}

LRESULT CALLBACK SystemWindow::messageProc(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_uMsg)
	{
		case WM_ACTIVATEAPP:
			break;
			
		case WM_CHAR:
			System::processedChar(_wParam);

			break;

		case WM_KEYDOWN:
		{
			Inputs	key	= InputNone;

			switch (_wParam)
			{
				case VK_LEFT:
					key	= InputLeft;

					break;

				case VK_RIGHT:
					key	= InputRight;

					break;

				case VK_UP:
					key	= InputUp;

					break;

				case VK_DOWN:
					key	= InputDown;

					break;

				case 'Z':
					key	= InputButton1;

					break;

				case 'X':
					key	= InputButton2;

					break;

				case VK_RETURN:
					key	= InputButtonStart;

					break;
			
				case VK_ESCAPE:
					key	= InputButtonExit;

					break;
			}

			System::keyDown(key);

			break;
		}
		
		case WM_KEYUP:
		{
			Inputs	key	= InputNone;

			switch (_wParam)
			{
				case VK_LEFT:
					key	= InputLeft;

					break;

				case VK_RIGHT:
					key	= InputRight;

					break;

				case VK_UP:
					key	= InputUp;

					break;

				case VK_DOWN:
					key	= InputDown;

					break;

				case 'Z':
					key	= InputButton1;

					break;

				case 'X':
					key	= InputButton2;

					break;

				case VK_RETURN:
					key	= InputButtonStart;

					break;
			
				case VK_ESCAPE:
					key	= InputButtonExit;

					break;
			}

			System::keyUp(key);

			break;
		}
		
		case WM_MOUSEMOVE:
			System::mouseMove(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam));

			break;

		case WM_LBUTTONDOWN:
			System::mouseButtonDown(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), MouseButtonLeft);

			break;

		case WM_LBUTTONUP:
			System::mouseButtonUp(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), MouseButtonLeft);
			
			break;

		case WM_MBUTTONDOWN:
			System::mouseButtonDown(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), MouseButtonMiddle);

			break;

		case WM_MBUTTONUP:
			System::mouseButtonUp(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), MouseButtonMiddle);
			
			break;

		case WM_RBUTTONDOWN:
			System::mouseButtonDown(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), MouseButtonRight);

			break;

		case WM_RBUTTONUP:
			System::mouseButtonUp(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), MouseButtonRight);
			
			break;

		case WM_LBUTTONDBLCLK:
			System::mouseDoubleClick(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), MouseButtonLeft);

			break;

		case WM_MBUTTONDBLCLK:
			System::mouseDoubleClick(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), MouseButtonMiddle);

			break;																		   
		case WM_RBUTTONDBLCLK:
			System::mouseDoubleClick(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), MouseButtonRight);

			break;

		case WM_MOUSEWHEEL:
			System::mouseWheel(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), GET_WHEEL_DELTA_WPARAM(_wParam));

			break;

		case WM_MOUSEHWHEEL:
			System::mouseHWheel(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam), GET_WHEEL_DELTA_WPARAM(_wParam));

			break;

		case WM_CLOSE:
			DestroyWindow(_hWnd);

			return	0;

		case WM_DESTROY:
			PostQuitMessage(0);

			break;
	
		case WM_PAINT:
		{
			PAINTSTRUCT	paintStruct;

			BeginPaint(_hWnd, &paintStruct);
			EndPaint(_hWnd, &paintStruct);

			return	TRUE;
		}

		case WM_ERASEBKGND:
			return	TRUE;

		case WM_COMMAND:
			break;
	}

	return	DefWindowProc(_hWnd, _uMsg, _wParam, _lParam);
}
