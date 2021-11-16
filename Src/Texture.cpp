// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Functions.h"
#include "Log.h"
#include "System.h"
#include "Texture.h"

Texture::Texture()	:
	m_uTextureID(GL_INVALID_VALUE),
	m_iImageWidth(0),
	m_iImageHeight(0),
	m_iTextureWidth(0),
	m_iTextureHeight(0),
	m_bDynamic(false)
{
}

Texture::~Texture()
{
	close();
}

Texture::Ptr Texture::create(int _iImageIndex, TextureFormat _eTextureFormat, bool _bDynamic)
{
	INSTANCE(pTexture, Texture())

	if (false == pTexture->initialize(_iImageIndex, _eTextureFormat, _bDynamic))
	{
		pTexture.reset();
	}

	return	pTexture;
}

bool Texture::initialize(int _iImageIndex, TextureFormat _eTextureFormat, bool _bDynamic)
{
	if (false == createTexture(_iImageIndex, _eTextureFormat, _bDynamic))
	{
		return	false;
	}
	
	return	true;
}

void Texture::close()
{
	if (m_uTextureID != GL_INVALID_VALUE)
	{
		glDeleteTextures(1, &m_uTextureID);

		m_uTextureID	= GL_INVALID_VALUE;
	}
}

void Texture::getSize(int& _iTextureWidth, int& _iTextureHeight) const
{
	_iTextureWidth	= m_iTextureWidth;
	_iTextureHeight	= m_iTextureHeight;
}

bool Texture::getBuffer(uint32_t*& _pBuffer, int& _iStride) const
{
	return	lockTexture(_pBuffer, _iStride);
}

void Texture::releaseBuffer()
{
	unlockTexture();
}

bool Texture::createTexture(int _iImageIndex, TextureFormat _eTextureFormat, bool _bDynamic)
{
	ResourceManager::Ptr	pResourceManager	= System::getResourceManager();

	if (nullptr == pResourceManager)
	{
		Log::instance()->logError("Resource manager is invalid");

		return	false;
	}

	Image::Ptr	pImage	= pResourceManager->getImage(_iImageIndex);

	if (nullptr == pImage)
	{
		Log::instance()->logError("Image is invalid");

		return	false;
	}

	m_iImageWidth	= pImage->getWidth();
	m_iImageHeight	= pImage->getHeight();
	m_bDynamic		= _bDynamic;

	m_iTextureWidth		= (int)Functions::getNextPowerOfTwo(m_iImageWidth);
	m_iTextureHeight	= (int)Functions::getNextPowerOfTwo(m_iImageHeight);

	int	iMaxTextureSize;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &iMaxTextureSize);

	if (m_iImageWidth > iMaxTextureSize || m_iImageHeight > iMaxTextureSize)
	{
		Log::instance()->logError("OpenGL cannot create a texture bigger than %d pixels", iMaxTextureSize);

		return	false;
	}

	int	iTextureSize	= m_iTextureWidth * m_iTextureHeight;

	SHAREDBUFFER(m_pTextureBuffer, uint32_t, iTextureSize);

	if (nullptr == m_pTextureBuffer)
	{
		Log::instance()->logError("Unable to allocate memory for texture buffer");

		return	false;
	}

	uint32_t*	pImagePtr	= pImage->getPixels();
	uint32_t*	pTexturePtr	= m_pTextureBuffer.get();

	memset(pTexturePtr, 0, iTextureSize * sizeof(uint32_t));

	for (int iY = 0; iY < m_iImageHeight; ++iY)
	{
		memcpy(pTexturePtr, pImagePtr, m_iImageWidth * sizeof(uint32_t));

		pTexturePtr	+= m_iTextureWidth;
		pImagePtr	+= m_iImageWidth;
	}
	
	m_uTextureID	= 0;

	if (false == generateTexture())
	{
		return	false;
	}

	if (false == setupTexture())
	{
		return	false;
	}

	return	true;
}
	 
bool Texture::lockTexture(uint32_t*& _pBuffer, int& _iStride) const
{
	_pBuffer	= m_pTextureBuffer.get();
	_iStride	= m_iTextureWidth;

	return	true;
}

bool Texture::unlockTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_uTextureID);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_iTextureWidth, m_iTextureHeight, gsc_textureFormat, GL_UNSIGNED_BYTE, m_pTextureBuffer.get());

	return	true;
}

bool Texture::generateTexture()
{
	glGenTextures(1, &m_uTextureID);

	GLenum	error	= glGetError();

	if (error != GL_NO_ERROR)
	{
		Log::instance()->logError("Unable to create texture %d", error);

		return	false;
	}

	glBindTexture(GL_TEXTURE_2D, m_uTextureID);

	error	= glGetError();

	if (error != GL_NO_ERROR)
	{
		Log::instance()->logError("Unable to bind texture %d", error);

		return	false;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return	true;
}

bool Texture::setupTexture()
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iTextureWidth, m_iTextureHeight, 0, gsc_textureFormat, GL_UNSIGNED_BYTE, m_pTextureBuffer.get());

	GLenum	error	= glGetError();

	if (error != GL_NO_ERROR)
	{
		Log::instance()->logError("Unable to setup texture %d", error);

		return	false;
	}

	return	true;
}
