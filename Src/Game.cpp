// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Game.h"
#include "Log.h"
#include "System.h"

Game::Game()	:
	m_iSpriteImage(-1)
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
	if (false == createSpriteBatch())
	{
		return	false;
	}

	if (false == loadSprites())
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
	if (m_pSpriteBatch != nullptr)
	{
		if (m_renderParams.iTextureHash != 0)
		{
			m_pSpriteBatch->startBatch();
			m_pSpriteBatch->drawSprite(m_renderParams);
			m_pSpriteBatch->endBatch();
		}
	}
}

bool Game::keyDown(Inputs _key)
{
	switch (_key)
	{
		case InputUp:
			m_renderParams.fY	-= 1.0f;

			break;

		case InputDown:
			m_renderParams.fY	+= 1.0f;

			break;
			
		case InputLeft:
			m_renderParams.fX	-= 1.0f;

			break;
			
		case InputRight:
			m_renderParams.fX	+= 1.0f;

			break;
			
		case InputButton1:
			m_renderParams.fWidth	*= 0.5f;
			m_renderParams.fHeight	*= 0.5f;
			
			break;
			
		case InputButton2:
			m_renderParams.fWidth	*= 2.0f;
			m_renderParams.fHeight	*= 2.0f;
			
			break;
			
		case InputButtonStart:
			m_renderParams.fWidth	= 16.0f;
			m_renderParams.fHeight	= 32.0f;

			break;
	}

	return	true;
}

bool Game::keyUp(Inputs _key)
{
	switch (_key)
	{
		case InputUp:
			break;

		case InputDown:
			break;
			
		case InputLeft:
			break;
			
		case InputRight:
			break;
			
		case InputButton1:
			break;
			
		case InputButton2:
			break;
			
		case InputButtonStart:
			break;
	}

	return	true;
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

bool Game::loadSprites()
{
	ResourceManager::Ptr	pResourceManager	= System::getResourceManager();
	Display::Ptr			pDisplay			= System::getDisplay();

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

	m_iSpriteImage	= pResourceManager->loadImage("Sprites/Skeleton.png");

	if (-1 == m_iSpriteImage)
	{
		return	false;
	}

	m_renderParams.iTextureHash	= pDisplay->createTexture("Sprites/Skeleton.png", m_iSpriteImage, Texture::Alpha8Bit);

	if (0 == m_renderParams.iTextureHash)
	{
		return	false;
	}

	Texture::Ptr	pTexture	= pDisplay->getTexture(m_renderParams.iTextureHash);

	if (nullptr == pTexture)
	{
		return	false;
	}

	m_renderParams.fX		= 160.0f;
	m_renderParams.fY		= 90.0f;
	m_renderParams.fWidth	= 16.0f;
	m_renderParams.fHeight	= 32.0f;
	m_renderParams.fU1		= 0.0f;
	m_renderParams.fV1		= 0.0f;
	m_renderParams.fU2		= pTexture->getU(16);
	m_renderParams.fV2		= pTexture->getV(32);

	return	true;
}

void Game::releaseSprites()
{
	if (m_renderParams.iTextureHash != 0)
	{
		Display::Ptr	pDisplay	= System::getDisplay();

		if (pDisplay != nullptr)
		{
			pDisplay->removeTexture(m_renderParams.iTextureHash);

			m_renderParams.iTextureHash	= 0;
		}
	}

	if (m_iSpriteImage != -1)
	{
		ResourceManager::Ptr	pResourceManager	= System::getResourceManager();

		if (pResourceManager != nullptr)
		{
			pResourceManager->releaseImage(m_iSpriteImage);
		}

		m_iSpriteImage	= -1;
	}
}
