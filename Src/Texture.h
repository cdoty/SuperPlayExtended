#pragma once

#include "Macros.h"
#include "SystemGL.h"

class Texture
{
	public:
		PTR(Texture)

		enum TextureFormat
		{
			NoAlpha,
			Alpha8Bit,
		};

		// Destructor
		~Texture();

		// Create
		static Ptr create(int _iImageIndex, TextureFormat _eTextureFormat, bool _bDynamic);

		// Initialize
		bool initialize(int _iImageIndex, TextureFormat _eTextureFormat, bool _bDynamic);
		
		// Close
		void close();

		// Get texture width and height
		void getSize(int& _iTextureWidth, int& _iTextureHeight) const;

		// Get image width
		int getImageWidth() const {return m_iImageWidth;}
		
		// Get image height
		int getImageHeight() const {return m_iImageHeight;}
		
		// Get texture width
		int getTextureWidth() const {return m_iTextureWidth;}
		
		// Get texture height
		int getTextureHeight() const {return m_iTextureHeight;}
		
		// Get texture
		GLuint getTexture() const {return m_uTextureID;}

		// Get buffer
		bool getBuffer(uint32_t*& _pBuffer, int& _iStride) const;
		
		// Release buffer
		void releaseBuffer();

		// Get U
		float getU(int _iX) const {return (float)_iX / (float)m_iTextureWidth;}

		// Get V
		float getV(int _iY) const {return (float)_iY / (float)m_iTextureHeight;}

	private:
		SHAREDPTR(uint32_t, m_pTextureBuffer);	// Texture buffer

		GLuint	m_uTextureID;		// OpenGL texture
		int		m_iImageWidth;		// Image width and height
		int		m_iImageHeight;
		int		m_iTextureWidth;	// Texture width and height
		int		m_iTextureHeight;
		bool	m_bDynamic;			// Dynamic

		// Constructor
		Texture();

		// Create texture
		bool createTexture(int _iImageIndex, TextureFormat _eTextureFormat, bool _bDynamic);

		// Lock texture
		bool lockTexture(uint32_t*& _pBuffer, int& _iStride) const;

		// Unlock texture
		bool unlockTexture();

		// Generate texture
		bool generateTexture();

		// Setup texture
		bool setupTexture();
};
