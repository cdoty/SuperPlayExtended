// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <chrono>

#include "Defines.h"
#include "File.h"
#include "Log.h"
#include "System.h"

Display::Ptr			System::ms_pDisplay;			// Display
Game::Ptr				System::ms_pGame;				// Game
ResourceManager::Ptr	System::ms_pResourceManager;	// Resource manager
Window::Ptr				System::ms_pWindow;				// Window
bool					System::ms_bExit	= false;	// Exit?

static const char*	gsc_szTranslationsDirectory	= "Translations";	// Translation directory

bool System::initialize()
{
	readSettings();

	ms_pWindow	= Window::create();

	if (nullptr == ms_pWindow)
	{
		return	false;
	}

	ms_pDisplay	= Display::create();

	if (nullptr == ms_pDisplay)
	{
		return	false;
	}

	ms_pResourceManager	= ResourceManager::create();

	if (nullptr == ms_pResourceManager)
	{
		return	false;
	}

	ms_pGame	= Game::create();

	if (nullptr == ms_pGame)
	{
		return	false;
	}

	return	true;
}

void System::close()
{
	ms_pGame.reset();
	ms_pResourceManager.reset();
	ms_pDisplay.reset();
	ms_pWindow.reset();
}

void System::run()
{
	if (ms_pWindow != nullptr)
	{
		auto lastTime	= std::chrono::steady_clock::now();

		while (true == ms_pWindow->update())
		{
			if (true == ms_bExit)
			{
				ms_pWindow->quit();
			}

			else
			{
				if (ms_pDisplay != nullptr)
				{
					ms_pDisplay->clear();
				}

				if (ms_pGame != nullptr)
				{
					auto currentTime	= std::chrono::steady_clock::now();
					
					std::chrono::duration<float>	elapsedTime	= currentTime - lastTime;

					ms_pGame->render(elapsedTime.count());

					lastTime	= currentTime;
				}

				if (ms_pDisplay != nullptr)
				{
					ms_pDisplay->present();
				}
			}
		}
	}
}

bool System::mouseButtonDown(int _x, int _y, Window::MouseButton _button)
{
	return	false;
}

bool System::mouseButtonUp(int _x, int _y, Window::MouseButton _button)
{
	return	false;
}

bool System::mouseDoubleClick(int _x, int _y, Window::MouseButton _button)
{
	return	false;
}
		
bool System::mouseMove(int _x, int _y)
{
	return	false;
}

bool System::mouseWheel(int _x, int _y, int _wheelRotation)
{
	return	false;
}

bool System::mouseHWheel(int _x, int _y, int _wheelRotation)
{
	return	false;
}

bool System::keyDown(int _key)
{
	return	false;
}

bool System::keyUp(int _key)
{
	return	false;
}

bool System::processedChar(uint32_t _char)
{
	return	false;
}

bool System::readSettings()
{
	return	true;
}
