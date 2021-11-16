#pragma once

#include  <X11/Xlib.h>

#include <EGL/egl.h>
#include <SDL2/SDL.h>

#include "Macros.h"
#include "WindowBase.h"

class SystemWindow : public WindowBase
{
	public:
		PTR(SystemWindow)
		SUPER(WindowBase)

		// Destructor
		~SystemWindow();

		// Create
		static Ptr create();

		// Initializeclea
		bool initialize();

		// Close
		void close();

		// Update
		bool update() override;

		// Quit
		void quit() override;

		// Get native window
		const Window& getNativeWindow() const {return m_window;}

	private:
		Display*	m_pDisplay;		// X window display
		Window		m_window;		// Window
		int			m_iXDirection;	// X direction
		int			m_iYDirection;	// X direction

		// Constructor
		SystemWindow();

		// Create window
		bool createWindow();
};
