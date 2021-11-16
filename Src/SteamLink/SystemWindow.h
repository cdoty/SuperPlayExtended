#pragma once

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

		// Get handle
		EGLNativeWindowType getHandle() const {return m_windowType;}
		
	private:
		SDL_Window*				m_pSDLWindow;		// SDL window
		EGLNativeDisplayType	m_displayType;		// Native display type
		EGLNativeWindowType		m_windowType;		// Native window type
		int						m_iXDirection;		// X direction
		int						m_iYDirection;		// X direction

		// Constructor
		SystemWindow();

		// Create window
		bool createWindow();
};
