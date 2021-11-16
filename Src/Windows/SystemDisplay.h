#pragma once

#include "DisplayBase.h"

class SystemDisplay : public DisplayBase
{
	public:
		PTR(SystemDisplay)
		SUPER(DisplayBase)

		// Create
		static Ptr create();

	private:
		// Constructor
		SystemDisplay();

		// Initialize EGL
		bool initializeEGL() override;
};
					 