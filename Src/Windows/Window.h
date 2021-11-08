#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <Windows.h>

#include "Macros.h"
#include "WindowBase.h"

class Window : public WindowBase
{
	public:
		enum MouseButton
		{
			MouseButtonLeft		= 1,
			MouseButtonMiddle	= 2,
			MouseButtonRight	= 4
		};

		PTR(Window)
		SUPER(WindowBase)

		// Destructor
		~Window();

		// Create
		static Ptr create();

		// Initialize
		bool initialize();

		// Close
		void close();

		// Update
		bool update() override;

		// Quit
		void quit() override;

		// Get window handle
		HWND getHwnd() const {return m_hWnd;}
		
		// Get width
		int getWidth() const {return m_rctMonitor.right - m_rctMonitor.left;}
		
		// Get height
		int getHeight() const {return m_rctMonitor.bottom - m_rctMonitor.top;}
		
		// Get position
		POINT getPosition() const {return POINT{m_rctMonitor.left, m_rctMonitor.top};}

		// Get rect
		const RECT& getRect() const {return m_rctWindow;}

		// Set monitor rect
		void setMonitorRect(const RECT& _rctMonitor);
		
		// Message proc
		static LRESULT CALLBACK messageProc(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam);

	private:
		HINSTANCE		m_hInstance;		// App instance
		HWND			m_hWnd;				// Window handle
		ATOM			m_classAtom;		// Window class atom
		std::wstring	m_strAppName;		// App name
		RECT			m_rctMonitor;		// Monitor rect
		RECT			m_rctWindow;		// Window rectangle

		// Constructor
		Window();
};
