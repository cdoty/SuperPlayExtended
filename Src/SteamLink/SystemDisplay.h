#pragma once

#include "DisplayBase.h"
#include "Macros.h"

class SystemDisplay : public DisplayBase
{
	public:
		PTR(SystemDisplay)
		SUPER(DisplayBase)

		// Destructor
		~SystemDisplay();

		// Create
		static Ptr create();

	private:
		// Constructor
		SystemDisplay();

		// Initialize EGL
		bool initializeEGL() override;
};
