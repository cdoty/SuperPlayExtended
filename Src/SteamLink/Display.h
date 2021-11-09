#pragma once

#include "DisplayBase.h"
#include "Macros.h"

class Display : public DisplayBase
{
	public:
		PTR(Display)
		SUPER(DisplayBase)

		// Destructor
		~Display();

		// Create
		static Ptr create();

	private:
		// Constructor
		Display();

		// Initialize EGL
		bool initializeEGL() override;
};
