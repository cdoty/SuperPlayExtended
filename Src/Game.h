#pragma once

#include "KeyDefines.h"
#include "Macros.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "TileMap.h"

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
		struct SpriteInfo
		{
			int		iTextureHash;	// Texture hash
			float	fX;				// Position
			float	fY;
			float	fWidth;			// Frame size
			float	fHeight;
			float	fU1;			// Frame UVs
			float	fV1;
			float	fU2;
			float	fV2;

			SpriteInfo() : iTextureHash(0), fX(0.0f), fY(0.0f), fWidth(0.0f), fHeight(0.0f), fU1(0.0f), fV1(0.0f), fU2(0.0f), fV2(0.0f) {}
		};

		SpriteBatch::Ptr	m_pSpriteBatch;		// Sprite batch
		Sprite::Ptr			m_pSprite;			// Sprite
		TileMap::Ptr		m_pTileMap;			// Tile map
		SpriteInfo			m_sprite;			// Sprite
		int					m_iSpriteImage;		// Sprite image
		uint32_t			m_controllerStatus;	// Controller status
		uint32_t			m_lastStatus;		// Last controller status

		// Update inputc
		void updateInput(float _fElapsed);

		// Create sprite batch
		bool createSpriteBatch();

		// Create sprite
		bool createSprite();

		// Create tile map
		bool createTileMap();

		// Load sprites
		bool loadSprites();

		// Release sprites
		void releaseSprites();
};
