#pragma once

#include "KeyDefines.h"
#include "Macros.h"
#include "SpriteBatch.h"

class Game 
{
	public:
		PTR(Game)

		// Constructor
		Game();

		// Destructor
		~Game();

		// Create
		static Ptr create();

		// Initialize
		bool initialize();

		// Close
		void close();

		// Render
		void render(float _fElapsed);

		// Key down
		bool keyDown(Inputs _key);

		// Key up
		bool keyUp(Inputs _key);

	private:
		SpriteBatch::Ptr	m_pSpriteBatch;		// Sprite batch
		RenderParams		m_renderParams;		// Render params
		int					m_iSpriteImage;		// Sprite image
		uint32_t			m_controllerStatus;	// Controller status
		uint32_t			m_lastStatus;		// Last controller status

		// Update inputc
		void updateInput(float _fElapsed);

		// Create sprite batch
		bool createSpriteBatch();

		// Load sprites
		bool loadSprites();

		// Release sprites
		void releaseSprites();
};
