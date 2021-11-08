#pragma once

#include <mutex>
#include <roku/RokuInput.h>

#include "Macros.h"
#include "WindowBase.h"

class Window : public WindowBase
{
	public:
		PTR(Window)
		SUPER(WindowBase)

		static int ms_iScreenWidth;		// Screen size
		static int ms_iScreenHeight;

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

	private:
		RokuInputContext*	m_pInputContext;	// Roku input context
		std::mutex			m_inputMutex;		// Input mutex

		// Constructor
		Window();

		// Create input context
		bool createInputContext();

		// Release input context
		void releaseInputContext();
};
