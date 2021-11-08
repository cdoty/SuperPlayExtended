#pragma once

#include "Display.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Window.h"

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
		static bool mouseButtonDown(int _x, int _y, Window::MouseButton _button);

		// Mouse button up
		static bool mouseButtonUp(int _x, int _y, Window::MouseButton _button);

		// Mouse button double click
		static bool mouseDoubleClick(int _x, int _y, Window::MouseButton _button);
		
		// Mouse move
		static bool mouseMove(int _x, int _y);

		// Mouse wheel
		static bool mouseWheel(int _x, int _y, int _wheelRotation);

		// Mouse h wheel
		static bool mouseHWheel(int _x, int _y, int _wheelRotation);

		// Key down
		static bool keyDown(int _key);

		// Key up
		static bool keyUp(int _key);

		// Processed char
		static bool processedChar(uint32_t _char);

		// Get display
		static Display::Ptr getDisplay() {return ms_pDisplay;}

		// Get game
		static Game::Ptr getGame() {return ms_pGame;}

		// Get resource manager
		static ResourceManager::Ptr getResourceManager() {return ms_pResourceManager;}

		// Get window
		static Window::Ptr getWindow() {return ms_pWindow;}

		// Get exit
		static bool getExit() {return ms_bExit;}

		// Set exit
		static void setExit(bool _bExit) {ms_bExit = _bExit;}

	private:
		static Display::Ptr			ms_pDisplay;			// Display
		static Game::Ptr			ms_pGame;				// Game
		static ResourceManager::Ptr	ms_pResourceManager;	// Resource manager
		static Window::Ptr			ms_pWindow;				// Window
		static bool					ms_bExit;				// Exit?

		// Read settings
		static bool readSettings();
};
