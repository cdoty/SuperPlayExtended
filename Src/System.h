#pragma once

#include "Game.h"
#include "KeyDefines.h"
#include "ResourceManager.h"
#include "SystemDisplay.h"
#include "SystemWindow.h"

class System
{
	public:
		// Initialize
		static bool initialize();

		// Close
		static void close();

		// Run
		static void run();

		// Mouse button down											   
		static bool mouseButtonDown(int _x, int _y, WindowBase::MouseButton _button);

		// Mouse button up
		static bool mouseButtonUp(int _x, int _y, WindowBase::MouseButton _button);

		// Mouse button double click
		static bool mouseDoubleClick(int _x, int _y, WindowBase::MouseButton _button);
		
		// Mouse move
		static bool mouseMove(int _x, int _y);

		// Mouse wheel
		static bool mouseWheel(int _x, int _y, int _wheelRotation);

		// Mouse h wheel
		static bool mouseHWheel(int _x, int _y, int _wheelRotation);

		// Key down
		static bool keyDown(Inputs _key);

		// Key up
		static bool keyUp(Inputs _key);

		// Processed char
		static bool processedChar(uint32_t _char);

		// Get game
		static Game::Ptr getGame() {return ms_pGame;}

		// Get resource manager
		static ResourceManager::Ptr getResourceManager() {return ms_pResourceManager;}

		// Get system display
		static SystemDisplay::Ptr getDisplay() {return ms_pDisplay;}
		
		// Get system window
		static SystemWindow::Ptr getWindow() {return ms_pWindow;}

		// Get exit
		static bool getExit() {return ms_bExit;}

		// Set exit
		static void setExit(bool _bExit) {ms_bExit = _bExit;}

	private:
		static Game::Ptr			ms_pGame;				// Game
		static ResourceManager::Ptr	ms_pResourceManager;	// Resource manager
		static SystemDisplay::Ptr	ms_pDisplay;			// Display
		static SystemWindow::Ptr	ms_pWindow;				// Window
		static bool					ms_bExit;				// Exit?

		// Read settings
		static bool readSettings();
};
