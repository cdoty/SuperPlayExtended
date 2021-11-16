// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Defines.h"
#include "File.h"
#include "Functions.h"
#include "Log.h"
#include "System.h"
#include "TileMap.h"

static const int	gsc_iSpriteVertices	= 4;	// Sprite vertices
static const int	gsc_iSpriteIndices	= 6;	// Sprite indices
static const int	gsc_iMapSize		= 2;	// Each map entry is 2 32 bit values

TileMap::TileMap(int _iTileMapWidth, int _iTileMapHeight, int _iTileWidth, int _iTileHeight)	:
	m_iTextureIndex(-1),
	m_fTileIncrementX(0.0f),
	m_fTileIncrementY(0.0f),
	m_iMaxSprites(_iTileMapWidth * _iTileMapHeight),
	m_iTileWidth(_iTileWidth),
	m_iTileHeight(_iTileHeight),
	m_iTileMapWidth(_iTileMapWidth),
	m_iTileMapHeight(_iTileMapHeight),
	m_iTextureTileX(1),
	m_iTextureTileY(1),
	m_iTextureImage(-1),
	m_iTextureHash(0),
	m_fTextureWidth(0.0f),
	m_fTextureHeight(0.0f)
{
}

TileMap::~TileMap()
{
	close();
}

TileMap::Ptr TileMap::create(const std::string& _strTexture, int _iTileMapWidth, int _iTileMapHeight, int _iTileWidth, int _iTileHeight)
{
	INSTANCE(pTileMap, TileMap(_iTileMapWidth, _iTileMapHeight, _iTileWidth, _iTileHeight))

	if (false == pTileMap->initialize(_strTexture))
	{
		pTileMap.reset();
	}

	return	pTileMap;
}

bool TileMap::initialize(const std::string& _strTexture)
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

void TileMap::close()
{
}

void TileMap::render()
{
	SystemDisplay::Ptr	pDisplay	= System::getDisplay();

	if (pDisplay != nullptr)
	{
		pDisplay->bindTexture(m_iTextureHash);

		// Draw the sprite batches
		pDisplay->drawTriangles(m_pVertexBuffer, m_iMaxSprites * gsc_iSpriteVertices, m_pIndexBuffer, m_iMaxSprites * 2);
	}
}

bool TileMap::setTileMap(const std::string& _strTileMap, int _iStartX, int _iStartY, int _iTileWidth, int _iTileHeight, int _iBlankTile)
{	
	if (_iStartX >= m_iTileMapWidth || _iStartY >= m_iTileMapHeight)
	{
		return	false;
	}

	if (false == loadTilemap(_strTileMap))
	{
		return	false;
	}

	if (nullptr == m_pTileMap)
	{
		Log::instance()->logError("Tilemap is invalid");

		return	false;
	}

	uint32_t*	pTileMap	= m_pTileMap.get();

	if (NULL == pTileMap)
	{
		Log::instance()->logError("Tilemap bufffer is invalid");

		return	false;
	}

	int	iEndX	= _iStartX + m_iTileMapWidth;
	int	iEndY	= _iStartY + m_iTileMapHeight;

	if (iEndX > _iTileWidth)
	{
		iEndX	= _iTileWidth;
	}

	if (iEndY > _iTileHeight)
	{
		iEndY	= _iTileHeight;
	}

	int	iStrideX	= m_iTileMapWidth - (iEndX - _iStartX);
	int	iMapStrideX	= _iTileWidth - (iEndX - _iStartX);

	pTileMap	+= (_iStartY * m_iTileMapWidth + _iStartX);

	int	iBlankTileX	= _iBlankTile % m_iTextureTileX;
	int	iBlankTileY	= _iBlankTile / m_iTextureTileX;

	int	iVertices;

	SHAREDTYPE(CustomVertex)	pVertexBufferMemory	= m_pVertexBuffer->getBuffer(iVertices);

	if (nullptr == pVertexBufferMemory)
	{
		Log::instance()->logError("Unable to get vertex buffer memory");

		return	false;
	}

	CustomVertex*	pVertexBuffer	= pVertexBufferMemory.get();

	if (_iBlankTile != -1)
	{
		float	fBlankU1	= (float)iBlankTileX / (float)m_iTextureTileX;
		float	fBlankV1	= (float)iBlankTileY / (float)m_iTextureTileY;
		float	fBlankU2	= fBlankU1 + m_fTileIncrementX;
		float	fBlankV2	= fBlankV1 + m_fTileIncrementY;

		for (int iLoop = 0; iLoop < m_iMaxSprites; ++iLoop)
		{
			pVertexBuffer[0].fU	= fBlankU1;
			pVertexBuffer[0].fV	= fBlankV2;
			pVertexBuffer[1].fU	= fBlankU2;
			pVertexBuffer[1].fV	= fBlankV2;
			pVertexBuffer[2].fU	= fBlankU1;
			pVertexBuffer[2].fV	= fBlankV1;
			pVertexBuffer[3].fU	= fBlankU2;
			pVertexBuffer[3].fV	= fBlankV1;

			pVertexBuffer	+= gsc_iSpriteVertices;
		}
	}

	pVertexBuffer	= pVertexBufferMemory.get();

	for (int iYLoop = _iStartY; iYLoop < iEndY; ++iYLoop)
	{
		for (int iXLoop = _iStartX; iXLoop < iEndX; ++iXLoop)
		{
			uint32_t	tile	= *pTileMap;

#ifdef USE_BIG_ENDIAN
			tile	= Functions::convertToBigEndian(tile);
#endif
			int	iTileX	= (int)(tile % m_iTextureTileX);
			int	iTileY	= (int)(tile / m_iTextureTileX);

			float	fSourceU1	= (float)iTileX / (float)m_iTextureTileX;
			float	fSourceV1	= (float)iTileY / (float)m_iTextureTileY;
			float	fSourceU2	= fSourceU1 + m_fTileIncrementX;
			float	fSourceV2	= fSourceV1 + m_fTileIncrementY;

			pVertexBuffer[0].fU	= fSourceU1;
			pVertexBuffer[0].fV	= fSourceV2;
			pVertexBuffer[1].fU	= fSourceU2;
			pVertexBuffer[1].fV	= fSourceV2;
			pVertexBuffer[2].fU	= fSourceU1;
			pVertexBuffer[2].fV	= fSourceV1;
			pVertexBuffer[3].fU	= fSourceU2;
			pVertexBuffer[3].fV	= fSourceV1;

			pTileMap++;
			pVertexBuffer	+= gsc_iSpriteVertices;
		}

		pVertexBuffer	+= gsc_iSpriteVertices * iStrideX;
		pTileMap		+= iMapStrideX;
	}

	return	true;
}

bool TileMap::loadProgram()
{
	return	true;
}

bool TileMap::loadTexture(const std::string& _strTexture)
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

	int	iTextureWidth	= pTexture->getTextureWidth();
	int	iTextureHeight	= pTexture->getTextureHeight();
	
	m_fTextureWidth		= (float)iTextureWidth;
	m_fTextureHeight	= (float)iTextureHeight;
	m_fTileIncrementX	= (float)m_iTileWidth / m_fTextureWidth;
	m_fTileIncrementY	= (float)m_iTileHeight / m_fTextureHeight;
	m_iTextureTileX		= iTextureWidth / m_iTileWidth;
	m_iTextureTileY		= iTextureHeight / m_iTileHeight;

	return	true;
}

bool TileMap::createBuffers()
{
	m_pIndexBuffer	= IndexBuffer::create(m_iMaxSprites * gsc_iSpriteIndices);

	if (nullptr == m_pIndexBuffer)
	{
		Log::instance()->logError("Unable to create index buffer");

		return	false;
	}

	int	iIndices;

	SHAREDTYPE(GLushort)	pIndexBufferMemory	= m_pIndexBuffer->getBuffer(iIndices);

	if (nullptr == pIndexBufferMemory)
	{
		Log::instance()->logError("Unable to get index buffer memory");

		return	false;
	}

	// Fill indices buffer
	uint16_t*	pIndexBuffer	= pIndexBufferMemory.get();
	int			iStartIndex		= 0;

	for (int iLoop = 0; iLoop < m_iMaxSprites; ++iLoop)
	{
		*pIndexBuffer	= iStartIndex;
		pIndexBuffer++;

		*pIndexBuffer	= iStartIndex + 1;
		pIndexBuffer++;

		*pIndexBuffer	= iStartIndex + 2;
		pIndexBuffer++;

		*pIndexBuffer	= iStartIndex + 1;
		pIndexBuffer++;

		*pIndexBuffer	= iStartIndex + 3;
		pIndexBuffer++;

		*pIndexBuffer	= iStartIndex + 2;
		pIndexBuffer++;

		iStartIndex	+= 4;
	}

	float	fTileWidth	= (float)m_iTileWidth;
	float	fTileHeight	= (float)m_iTileHeight;

	float	fDestY1		= 0.0f;
	float	fSourceU1	= 0.0f;
	float	fSourceV1	= 0.0f;
	float	fSourceU2	= fSourceU1 + m_fTileIncrementX;
	float	fSourceV2	= fSourceV1 + m_fTileIncrementY;

	m_pVertexBuffer	= VertexBuffer::create(m_iMaxSprites * gsc_iSpriteVertices, true);

	if (nullptr == m_pVertexBuffer)
	{
		Log::instance()->logError("Unable to create vertex buffer");

		return	false;
	}

	int	iVertices;

	SHAREDTYPE(CustomVertex)	pVertexBufferMemory	= m_pVertexBuffer->getBuffer(iVertices);

	if (nullptr == pVertexBufferMemory)
	{
		Log::instance()->logError("Unable to get vertex buffer memory");

		return	false;
	}

	CustomVertex*	pVertexBuffer	= pVertexBufferMemory.get();

	for (int iYLoop = 0; iYLoop < m_iTileMapHeight; ++iYLoop)
	{
		float	fDestX1	= 0.0f;

		for (int iXLoop = 0; iXLoop < m_iTileMapWidth; ++iXLoop)
		{
			float	fDestX2		= fDestX1 + (float)fTileWidth;
			float	fDestY2		= fDestY1 + (float)fTileHeight;

			pVertexBuffer[0]	= {fDestX1, fDestY2, fSourceU1, fSourceV2};
			pVertexBuffer[1]	= {fDestX2, fDestY2, fSourceU2, fSourceV2};
			pVertexBuffer[2]	= {fDestX1, fDestY1, fSourceU1, fSourceV1};
			pVertexBuffer[3]	= {fDestX2, fDestY1, fSourceU2, fSourceV1};

			fDestX1	+= fTileWidth;

			pVertexBuffer	+= gsc_iSpriteVertices;
		}

		fDestY1	+= fTileHeight;
	}

	SHAREDBUFFER(m_pTileMap, uint32_t, m_iMaxSprites);
	
	memset(m_pTileMap.get(), 0, m_iMaxSprites);

	return	true;
}

bool TileMap::loadTilemap(const std::string& _strTileMap)
{
	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		return	false;
	}

	if (false == pFile->open(_strTileMap, true))
	{
		return	false;
	}

	int	iLength	= pFile->getLength();
	int	iTiles;

	if (iLength / (int)(sizeof(uint32_t) * gsc_iMapSize) > m_iMaxSprites)
	{
		iTiles	= m_iMaxSprites;
	}

	else
	{
		iTiles	= iLength / (sizeof(uint32_t) * gsc_iMapSize);
	}

	uint32_t*	pBuffer	= m_pTileMap.get();

	for (int iLoop = 0; iLoop < iTiles; ++iLoop)
	{
		uint32_t	tile;

		if (false == pFile->readUnsigned32Bit(tile))
		{
			return	false;
		}

#if defined USE_BIG_ENDIAN
		tile	= Functions::convertToBigEndian(tile);
#endif

		*pBuffer	= tile;
		pBuffer++;

		// Read flip value
		if (false == pFile->readUnsigned32Bit(tile))
		{
			return	false;
		}
	}

	pFile->close();

	return	true;
}
