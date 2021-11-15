// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Defines.h"
#include "Log.h"
#include "Sprite.h"
#include "System.h"

static const float	gsc_fNearPlane		= 0.0f;		// Near plane
static const float	gsc_fFarPlane		= 100.0f;	// Far plane
static const int	gsc_iSpriteVertices	= 4;		// Sprite vertices
static const int	gsc_iSpriteIndices	= 6;		// Sprite indices

Sprite::Sprite()	:
	m_iTextureImage(-1),
	m_iTextureHash(0)
{
}

Sprite::~Sprite()
{
	close();
}

Sprite::Ptr Sprite::create(const std::string& _strTexture)
{
	INSTANCE(pSprite, Sprite())

	if (false == pSprite->initialize(_strTexture))
	{
		pSprite.reset();
	}
	
	return	pSprite;
}

bool Sprite::initialize(const std::string& _strTexture)
{
	if (false == loadProgram())
	{
		return	false;
	}

	if (false == loadTexture(_strTexture))
	{
		return	false;
	}

	if (false == createBuffers())
	{
		return	false;
	}

	return	true;
}

void Sprite::close()
{
}

bool Sprite::draw(int _iDestX, int _iDestY, int _iDestWidth, int _iDestHeight, int _iSourceX, int _iSourceY, int _iSourceWidth, int _iSourceHeight)
{
	if (-1 == m_iTextureHash)
	{
		Log::instance()->logError("Texture is invalid");

		return	false;
	}

	Display::Ptr	pDisplay	= System::getDisplay();

	if (nullptr == pDisplay)
	{
		Log::instance()->logError("Display is invalid");

		return	false;
	}

	Texture::Ptr	pTexture	= pDisplay->getTexture(m_iTextureHash);

	if (nullptr == pTexture)
	{
		Log::instance()->logError("Texture is invalid");

		return	false;
	}

	int	iVertices;

	SHAREDTYPE(CustomVertex)	pVertextBufferMemory = m_pVertexBuffer->getBuffer(iVertices);

	if (nullptr == pVertextBufferMemory)
	{
		Log::instance()->logError("Vertex buffer memory is invalid");

		return	false;
	}

	CustomVertex*	pBuffer	= pVertextBufferMemory.get();

	float	fDestX1		= (float)_iDestX;
	float	fDestY1		= (float)_iDestY;
	float	fDestX2		= fDestX1 + (float)_iDestWidth;
	float	fDestY2		= fDestY1 + (float)_iDestHeight;

	float	fSourceU1	= pTexture->getU(_iSourceX);
	float	fSourceV1	= pTexture->getV(_iSourceY);
	float	fSourceU2	= pTexture->getU(_iSourceX + _iSourceWidth);
	float	fSourceV2	= pTexture->getV(_iSourceY + _iSourceHeight);

	pBuffer[0]	= {fDestX1, fDestY2, fSourceU1, fSourceV2};
	pBuffer[1]	= {fDestX2, fDestY2, fSourceU2, fSourceV2};
	pBuffer[2]	= {fDestX1, fDestY1, fSourceU1, fSourceV1};
	pBuffer[3]	= {fDestX2, fDestY1, fSourceU2, fSourceV1};

	render();

	return	true;
}

bool Sprite::drawCentered(int _iCenterDestX, int _iCenterDestY, int _iDestWidth, int _iDestHeight, int _iSourceX, int _iSourceY, int _iSourceWidth, 
	int _iSourceHeight)
{
	if (-1 == m_iTextureHash)
	{
		Log::instance()->logError("Texture is invalid");

		return	false;
	}

	Display::Ptr	pDisplay	= System::getDisplay();

	if (nullptr == pDisplay)
	{
		Log::instance()->logError("Display is invalid");

		return	false;
	}

	Texture::Ptr	pTexture	= pDisplay->getTexture(m_iTextureHash);

	if (nullptr == pTexture)
	{
		Log::instance()->logError("Texture is invalid");

		return	false;
	}

	int	iVertices;

	SHAREDTYPE(CustomVertex)	pVertextBufferMemory = m_pVertexBuffer->getBuffer(iVertices);

	if (nullptr == pVertextBufferMemory)
	{
		Log::instance()->logError("Vertex buffer memory is invalid");

		return	false;
	}

	CustomVertex*	pBuffer	= pVertextBufferMemory.get();

	float	fDestX1		= (float)_iCenterDestX - (float)_iDestWidth / 2.0f;
	float	fDestY1		= (float)_iCenterDestY - (float)_iDestHeight / 2.0f;
	float	fDestX2		= fDestX1 + (float)_iDestWidth;
	float	fDestY2		= fDestY1 + (float)_iDestHeight;

	float	fSourceU1	= pTexture->getU(_iSourceX);
	float	fSourceV1	= pTexture->getV(_iSourceY);
	float	fSourceU2	= pTexture->getU(_iSourceX + _iSourceWidth);
	float	fSourceV2	= pTexture->getV(_iSourceY + _iSourceHeight);

	pBuffer[0]	= {fDestX1, fDestY2, fSourceU1, fSourceV2};
	pBuffer[1]	= {fDestX2, fDestY2, fSourceU2, fSourceV2};
	pBuffer[2]	= {fDestX1, fDestY1, fSourceU1, fSourceV1};
	pBuffer[3]	= {fDestX2, fDestY1, fSourceU2, fSourceV1};

	render();

	return	true;
}

bool Sprite::loadProgram()
{
	return	true;
}

bool Sprite::loadTexture(const std::string& _strTexture)
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

	m_iTextureImage	= pResourceManager->createImage(_strTexture);

	if (-1 == m_iTextureImage)
	{
		return	false;
	}

	m_iTextureHash	= pDisplay->createTexture(_strTexture, m_iTextureImage, Texture::Alpha8Bit);

	if (0 == m_iTextureHash)
	{
		return	false;
	}

	Texture::Ptr	pTexture	= pDisplay->getTexture(m_iTextureHash);

	if (nullptr == pTexture)
	{
		return	false;
	}

	return	true;
}

bool Sprite::createBuffers()
{
	m_pIndexBuffer	= IndexBuffer::create(gsc_iSpriteIndices);

	if (nullptr == m_pIndexBuffer)
	{
		Log::instance()->logError("Unable to create index buffer");

		return	false;
	}

	int	iIndices;

	// Fill indices buffer
	SHAREDTYPE(GLushort)	pIndexBufferMemory	= m_pIndexBuffer->getBuffer(iIndices);

	if (nullptr == pIndexBufferMemory)
	{
		Log::instance()->logError("Index buffer memory is invalid");

		return	false;
	}

	GLushort*	pBuffer	= pIndexBufferMemory.get();
	
	*pBuffer	= 0;
	pBuffer++;

	*pBuffer	= 1;
	pBuffer++;

	*pBuffer	= 2;
	pBuffer++;

	*pBuffer	= 1;
	pBuffer++;

	*pBuffer	= 3;
	pBuffer++;

	*pBuffer	= 2;
	pBuffer++;

	m_pVertexBuffer	= VertexBuffer::create(gsc_iSpriteVertices, true);

	if (nullptr == m_pVertexBuffer)
	{
		Log::instance()->logError("Unable to create index buffer");

		return	false;
	}

	return	true;
}

void Sprite::render()
{
	Display::Ptr	pDisplay	= System::getDisplay();

	if (pDisplay != nullptr)
	{
		Display::Ptr pDisplay	= System::getDisplay();

		pDisplay->bindTexture(m_iTextureHash);

		// Draw the sprite batches
		pDisplay->drawTriangles(m_pVertexBuffer, 4, m_pIndexBuffer, 2);
	}
}
