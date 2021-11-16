#pragma once

#include <mutex>
#include <roku/RokuInput.h>

#include "Macros.h"
#include "WindowBase.h"

class SystemWindow : public WindowBase
{
	public:
		PTR(SystemWindow)
		SUPER(WindowBase)

		static int ms_iScreenWidth;		// Screen size
		static int ms_iScreenHeight;

		// Destructor
		~SystemWindow();

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
		SystemWindow();

		// Create input context
		bool createInputContext();

		// Release input context
		void releaseInputContext();
};
