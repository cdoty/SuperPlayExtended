// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Game.h"
#include "Log.h"
#include "System.h"

static const float		gsc_fMoveSpeed	= 32.0f;		// Move 16 pixels per second
static const uint32_t	gsc_borderColor	= 0xFF000000;	// Border color
static const uint32_t	gsc_clearColor	= 0xFF0000FF;	// Clear color

Game::Game()	:
	m_iSpriteImage(-1),
	m_controllerStatus(0),
	m_lastStatus(0)
{
}

Game::~Game()
{
	close();
}

Game::Ptr Game::create()
{
	INSTANCE(pGame, Game())

	if (false == pGame->initialize())
	{
		pGame.reset();
	}

	return	pGame;
}

bool Game::initialize()
{
	SystemDisplay::Ptr	pDisplay	= System::getDisplay();

	if (nullptr == pDisplay)
	{
		Log::instance()->logError("Display is invalid");

		return	false;
	}

	pDisplay->setBorderColor(gsc_borderColor);
	pDisplay->setClearColor(gsc_clearColor);

	if (false == createSpriteBatch())
	{
		return	false;
	}

	if (false == createSprite())
	{
		return	false;
	}

	if (false == loadSprites())
	{
		return	false;
	}

	if (false == createTileMap())
	{
		return	false;
	}

	return	true;
}

void Game::close()
{
	releaseSprites();
}

void Game::render(float _fElapsed)
{
	updateInput(_fElapsed);

	if (m_pTileMap != nullptr)
	{
		m_pTileMap->render();
	}

	if (m_pSpriteBatch != nullptr)
	{
		if (m_sprite.iTextureHash != 0)
		{
			m_pSpriteBatch->startBatch();
			m_pSpriteBatch->drawSprite(m_sprite.iTextureHash, m_sprite.fX, m_sprite.fY, m_sprite.fWidth, m_sprite.fHeight, m_sprite.fU1, m_sprite.fV1, 
				m_sprite.fU2, m_sprite.fV2);

			m_pSpriteBatch->endBatch();
		}

		if (m_pSprite != nullptr)
		{
			m_pSprite->draw(320 / 2 - 16, 180 - 32, 32, 32, 0, 96, 32, 32);
		}
	}
}

bool Game::keyDown(Inputs _key)
{
	m_controllerStatus	|= _key;

	return	true;
}

bool Game::keyUp(Inputs _key)
{
	m_controllerStatus	&= ~_key;

	return	true;
}

void Game::updateInput(float _fElapsed)
{
	if (0 == (m_lastStatus & InputButton1) && m_controllerStatus & InputButton1)
	{
		m_sprite.fWidth		*= 0.5f;
		m_sprite.fHeight	*= 0.5f;
	}
			
	if (0 == (m_lastStatus & InputButton2) && m_controllerStatus & InputButton2)
	{
		m_sprite.fWidth		*= 2.0f;
		m_sprite.fHeight	*= 2.0f;
	}			
			
	if (0 == (m_lastStatus & InputButtonStart) && m_controllerStatus & InputButtonStart)
	{
		m_sprite.fWidth		= 16.0f;
		m_sprite.fHeight	= 32.0f;
	}

	if (0 == (m_lastStatus & InputButtonExit) && m_controllerStatus & InputButtonExit)
	{
		System::setExit(true);
	}

	float	fMovementSpeed	= gsc_fMoveSpeed * (m_sprite.fWidth / 16.0f);

	if (m_controllerStatus & InputUp)
	{
		m_sprite.fY	-= fMovementSpeed * _fElapsed;
	}

	else if (m_controllerStatus & InputDown)
	{
		m_sprite.fY	+= fMovementSpeed * _fElapsed;
	}

	if (m_controllerStatus & InputLeft)
	{
		m_sprite.fX	-= fMovementSpeed * _fElapsed;
	}

	else if (m_controllerStatus & InputRight)
	{
		m_sprite.fX	+= fMovementSpeed * _fElapsed;
	}

	m_lastStatus	= m_controllerStatus;
}

bool Game::createSpriteBatch()
{
	m_pSpriteBatch	= SpriteBatch::create();

	if (nullptr == m_pSpriteBatch)
	{
		return	false;
	}

	return	true;
}

bool Game::createSprite()
{
	m_pSprite	= Sprite::create("Sprites/Player.png");

	if (nullptr == m_pSprite)
	{
		return	false;
	}

	return	true;
}

bool Game::createTileMap()
{
	m_pTileMap	= TileMap::create("Maps/Map.png", 10, 30, 32, 32);
	
	if (nullptr == m_pTileMap)
	{
		return	false;
	}

	m_pTileMap->setTileMap("Maps/Map.map", 0, 0);

	return	true;
}

bool Game::loadSprites()
{
	ResourceManager::Ptr	pResourceManager	= System::getResourceManager();
	SystemDisplay::Ptr		pDisplay			= System::getDisplay();

	if (nullptr == pResourceManager)
	{
		Log::instance()->logError("Resource Manager is invalid");

		return	false;
	}

	if (nullptr == pDisplay)
	{
		Log::instance()->logError("Display is invalid");

		return	false;
	}

	m_iSpriteImage	= pResourceManager->createImage("Sprites/Skeleton.png");

	if (-1 == m_iSpriteImage)
	{
		return	false;
	}

	m_sprite.iTextureHash	= pDisplay->createTexture("Sprites/Skeleton.png", m_iSpriteImage, Texture::Alpha8Bit);

	if (0 == m_sprite.iTextureHash)
	{
		return	false;
	}

	Texture::Ptr	pTexture	= pDisplay->getTexture(m_sprite.iTextureHash);

	if (nullptr == pTexture)
	{
		return	false;
	}

	m_sprite.fX			= 160.0f;
	m_sprite.fY			= 90.0f;
	m_sprite.fWidth		= 16.0f;
	m_sprite.fHeight	= 32.0f;
	m_sprite.fU1		= 0.0f;
	m_sprite.fV1		= 0.0f;
	m_sprite.fU2		= pTexture->getU(16);
	m_sprite.fV2		= pTexture->getV(32);

	return	true;
}

void Game::releaseSprites()
{
	if (m_sprite.iTextureHash != 0)
	{
		SystemDisplay::Ptr	pDisplay	= System::getDisplay();

		if (pDisplay != nullptr)
		{
			pDisplay->removeTexture(m_sprite.iTextureHash);

			m_sprite.iTextureHash	= 0;
		}
	}

	if (m_iSpriteImage != -1)
	{
		ResourceManager::Ptr	pResourceManager	= System::getResourceManager();

		if (pResourceManager != nullptr)
		{
			pResourceManager->removeImage(m_iSpriteImage);
		}

		m_iSpriteImage	= -1;
	}
}
