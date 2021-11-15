#pragma once

#include "CustomVertex.h"
#include "Macros.h"
#include "Rect.h"
#include "VertexBuffer.h"

class SpriteBatch
{
	public:
		PTR(SpriteBatch)

		// Destructor
		~SpriteBatch();

		// Create
		static Ptr create(int _iSprites = 64);

		// Initialize
		bool initialize(int _iSprites = 64);
		
		// Close
		void close();
		
		// Start batch
		void startBatch(const Rect& _rctClip);
		void startBatch();

		// Draw sprite
		void drawSprite(int _iTextureHash, float _fX, float _fY, float _fWidth, float _fHeight, float _fU1, float _fV1, float _fU2, float _fV2);

		// End batch
		void endBatch();

	protected:
		VertexBuffer::Ptr	m_pVertexBuffer;		// Temp vertex buffer pointer, used in between begin and end batch calls
		CustomVertex*		m_pBuffer;				// Temp custom vertex buffer pointer
		int					m_iVertexBufferIndex;	// Vertex buffer index
		int					m_iIndexBufferIndex;	// Index buffer index
		int					m_iTextureIndex;		// Current texture index
		int					m_iSpriteIndex;			// Current sprite index
		int					m_iMaxSprites;			// Sprite batch maximum sprites

		// Constructor
		SpriteBatch();

		// Draw batch
		void drawBatch();

		// Get vertex buffer
		bool getVertexBuffer();

		// Release vertex buffer
		void releaseVertexBuffer();
};
