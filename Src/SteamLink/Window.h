#pragma once

#include <EGL/egl.h>
#include <SDL2/SDL.h>

#include "Macros.h"
#include "WindowBase.h"

class Window : public WindowBase
{
	public:
		PTR(Window)
		SUPER(WindowBase)

		// Destructor
		~Window();

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
		SDL_Window*				m_pSDLWindow;	// SDL window
		EGLNativeDisplayType	m_displayType;	// Native display type
		EGLNativeWindowType		m_windowType;	// Native window type

		// Constructor
		Window();

		// Create window
		bool createWindow();
};
