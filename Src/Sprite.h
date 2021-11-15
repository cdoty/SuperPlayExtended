#pragma once

#include "IndexBuffer.h"
#include "Macros.h"
#include "Program.h"
#include "VertexBuffer.h"

class Sprite
{
	public:
		PTR(Sprite)
		
		// Destructor
		~Sprite();

		// Create
		static Ptr create(const std::string& _strTexture);

		// Initialize
		bool initialize(const std::string& _strTexture);

		// Close
		void close();

		// Draw
		bool draw(int _iDestX, int _iDestY, int _iDestWidth, int _iDestHeight, int _iSourceX, int _iSourceY, int _iSourceWidth, int _iSourceHeight);

		// Draw
		bool drawCentered(int _iCenterDestX, int _iCenterDestY, int _iDestWidth, int _iDestHeight, int _iSourceX, int _iSourceY, int _iSourceWidth, 
			int _iSourceHeight);

	private:					
		IndexBuffer::Ptr	m_pIndexBuffer;		// Index buffer
		Program::Ptr		m_pProgram;			// Program
		VertexBuffer::Ptr	m_pVertexBuffer;	// Vertex buffer
		int					m_iTextureImage;	// Texture image
		int					m_iTextureHash;		// Texture hash

		// Constructor
		Sprite();

		// Load program
		bool loadProgram();

		// Load texture
		bool loadTexture(const std::string& _strTexture);

		// Create buffers
		bool createBuffers();

		// Render
		void render();
};
