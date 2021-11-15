// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CustomVertex.h"
#include "Display.h"
#include "Log.h"
#include "SpriteBatch.h"
#include "System.h"

SpriteBatch::SpriteBatch()	:
	m_pBuffer(NULL),
	m_iVertexBufferIndex(-1),
	m_iIndexBufferIndex(-1),
	m_iTextureIndex(-1),
	m_iSpriteIndex(0),
	m_iMaxSprites(0)
{
}

SpriteBatch::~SpriteBatch()
{
	close();
}

SpriteBatch::Ptr SpriteBatch::create(int _iSprites)
{
	INSTANCE(pSpriteBatch, SpriteBatch())

	if (false == pSpriteBatch->initialize(_iSprites))
	{
		pSpriteBatch.reset();
	}

	return	pSpriteBatch;
}

bool SpriteBatch::initialize(int _iSprites)
{
	m_iMaxSprites	= _iSprites;
	
	Display::Ptr	pDisplay	= System::getDisplay();

	if (nullptr == pDisplay)
	{
		Log::instance()->logError("Display is invalid");

		return	false;
	}
	
	// Create vertex buffer
	m_iVertexBufferIndex	= pDisplay->createVertexBuffer(_iSprites * 4, true);
	
	// Create index buffer
	m_iIndexBufferIndex		= pDisplay->createIndexBuffer(_iSprites * 6, false);

	IndexBuffer::Ptr	pIndexBuffer	= pDisplay->getIndexBuffer(m_iIndexBufferIndex);

	if (nullptr == pIndexBuffer)
	{
		Log::instance()->logError("Index buffer is invalid");

		return	false;
	}

	int	iIndices	= 0;
	
	GLushort*	pBuffer	= pIndexBuffer->getBuffer(iIndices).get();

	if (NULL == pBuffer)
	{
		Log::instance()->logError("Index buffer memory is invalid");

		return	false;
	}

	// Pre assign indices
	for (int iLoop = 0; iLoop < _iSprites; ++iLoop)
	{
		int	iIndex			= iLoop * 4;
		int	iBufferIndex	= iLoop * 6;
		
		pBuffer[iBufferIndex++]	= iIndex;
		pBuffer[iBufferIndex++]	= iIndex + 1;
		pBuffer[iBufferIndex++]	= iIndex + 2;
		pBuffer[iBufferIndex++]	= iIndex + 1;
		pBuffer[iBufferIndex++]	= iIndex + 3;
		pBuffer[iBufferIndex++]	= iIndex + 2;
	}

	pIndexBuffer->releaseBuffer();
	
	return	true;
}

void SpriteBatch::close()
{
	Display::Ptr pDisplay	= System::getDisplay();

	if (pDisplay != nullptr)
	{
		if (m_iVertexBufferIndex != -1)
		{
			pDisplay->removeVertexBuffer(m_iVertexBufferIndex);

			m_iVertexBufferIndex	= -1;
		}

		if (m_iIndexBufferIndex != -1)
		{
			pDisplay->removeIndexBuffer(m_iIndexBufferIndex);

			m_iIndexBufferIndex	= -1;
		}
	}
}

void SpriteBatch::startBatch(const Rect& _rctClip)
{
	if (_rctClip.iWidth > 0 && _rctClip.iHeight > 0)
	{
		Display::Ptr pDisplay	= System::getDisplay();

		if (pDisplay != nullptr)
		{
			pDisplay->setClipRect(_rctClip);
		}
	}

	startBatch();
}

void SpriteBatch::startBatch()
{
	m_iTextureIndex	= -1;
	m_iSpriteIndex	= 0;
}

void SpriteBatch::drawSprite(int _iTextureHash, float _fX, float _fY, float _fWidth, float _fHeight, float _fU1, float _fV1, float _fU2, float _fV2)
{
	// If changing texture, draw sprite batch
	if (m_iTextureIndex != _iTextureHash)
	{
		m_iTextureIndex	= _iTextureHash;

		Display::Ptr pDisplay	= System::getDisplay();

		if (pDisplay != nullptr)
		{
			pDisplay->bindTexture(m_iTextureIndex);
		}

		// Draw all sprite batches associated with the existing texture
		drawBatch();

		getVertexBuffer();
	}
	
	if (m_pBuffer != NULL)
	{
		int	iIndex	= m_iSpriteIndex << 2;

		float	fX1	= _fX;
		float	fY1	= _fY;
		float	fX2	= fX1 + _fWidth;
		float	fY2	= fY1 + _fHeight;

		CustomVertex*	pBuffer	= &m_pBuffer[iIndex];

		// (0)*--*(2)
		//    | /|
		// (1)*--*(3)
		pBuffer->fX	= fX1;
		pBuffer->fY	= fY1;

		pBuffer->fU	= _fU1;
		pBuffer->fV	= _fV1;
	
		pBuffer++;

		pBuffer->fX	= fX1;
		pBuffer->fY	= fY2;

		pBuffer->fU	= _fU1;
		pBuffer->fV	= _fV2;
	
		pBuffer++;

		pBuffer->fX	= fX2;
		pBuffer->fY	= fY1;

		pBuffer->fU	= _fU2;
		pBuffer->fV	= _fV1;
	
		pBuffer++;

		pBuffer->fX	= fX2;
		pBuffer->fY	= fY2;

		pBuffer->fU	= _fU2;
		pBuffer->fV	= _fV2;
	
		m_iSpriteIndex++;

		if (m_iSpriteIndex >= m_iMaxSprites)
		{
			drawBatch();

			getVertexBuffer();
		}
	}
}

void SpriteBatch::endBatch()
{
	// Draw any remaining sprite batches.
	drawBatch();

	Display::Ptr	pDisplay	= System::getDisplay();

	if (pDisplay != nullptr)
	{
		pDisplay->resetClipRect();
	}
}

void SpriteBatch::drawBatch()
{
	releaseVertexBuffer();

	if (m_iSpriteIndex > 0)
	{
		Display::Ptr	pDisplay	= System::getDisplay();

		if (pDisplay != nullptr)
		{
			// Draw the sprite batches
			pDisplay->drawTriangles(m_iVertexBufferIndex, m_iSpriteIndex * 4, m_iIndexBufferIndex, m_iSpriteIndex * 2);
		}
		
		m_iSpriteIndex	= 0;
	}
}

bool SpriteBatch::getVertexBuffer()
{
	Display::Ptr	pDisplay	= System::getDisplay();

	if (nullptr == pDisplay)
	{
		m_pBuffer	= NULL;

		Log::instance()->logError("Display is invalid");

		return	false;
	}

	m_pVertexBuffer	= pDisplay->getVertexBuffer(m_iVertexBufferIndex);

	int	iVertices	= 0;
	
	m_pBuffer	= (CustomVertex*)m_pVertexBuffer->getBuffer(iVertices).get();

	return	true;
}

void SpriteBatch::releaseVertexBuffer()
{
	if (m_pVertexBuffer != NULL)
	{
		m_pVertexBuffer->releaseBuffer();

		m_pBuffer		= NULL;
	}
}
