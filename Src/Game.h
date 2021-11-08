#pragma once

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

	private:
		SpriteBatch::Ptr	m_pSpriteBatch;	// Sprite batch
		RenderParams		m_renderParams;	// Render params
		int					m_iSpriteImage;	// Sprite image

		// Create sprite batch
		bool createSpriteBatch();

		// Load sprites
		bool loadSprites();

		// Release sprites
		void releaseSprites();
};
