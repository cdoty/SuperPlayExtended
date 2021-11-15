#pragma once

#include "CustomVertex.h"
#include "IndexBuffer.h"
#include "Macros.h"
#include "Program.h"
#include "VertexBuffer.h"

class TileMap
{
	public:
		PTR(TileMap)

		// Destructor
		~TileMap();

		// Create
		static Ptr create(const std::string& _strTexture, int _iTileMapWidth, int _iTileMapHeight, int _iTileWidth, int _iTileHeight);

		// Initialize
		bool initialize(const std::string& _strTexture);
		
		// Close
		void close();

		// Render
		void render();

		// Set tilemap
		bool setTileMap(const std::string& _strTileMap, int _iStartX, int _iStartY, int _iBlankTile = -1);

	private:
		SHAREDPTR(uint32_t, m_pTileMap);

		IndexBuffer::Ptr	m_pIndexBuffer;		// Index buffer
		Program::Ptr		m_pProgram;			// Program
		VertexBuffer::Ptr	m_pVertexBuffer;	// Vertex buffer
		int					m_iTextureIndex;	// Current texture index
		float				m_fTileIncrementX;	// Tile increment X
		float				m_fTileIncrementY;	// Tile increment Y
		int					m_iMaxSprites;		// Max sprites
		int					m_iTileWidth;		// Tile size
		int					m_iTileHeight;
		int					m_iTileMapWidth;	// Tile map size
		int					m_iTileMapHeight;
		int					m_iTextureTileX;	// Texture tiles
		int					m_iTextureTileY;
		int					m_iTextureImage;	// Texture image
		int					m_iTextureHash;		// Texture hash
		float				m_fTextureWidth;	// Texture width
		float				m_fTextureHeight;	// Texture height

		// Constructor
		TileMap(int _iTileMapWidth, int _iTileMapHeight, int _iTileWidth, int _iTileHeight);

		// Load program
		bool loadProgram();

		// Load texture
		bool loadTexture(const std::string& _strTexture);

		// Create buffers
		bool createBuffers();

		// Load tilemap
		bool loadTilemap(const std::string& _strTileMap);
};