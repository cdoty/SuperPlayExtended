// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "File.h"
#include "Functions.h"
#include "Image.h"
#include "Log.h"
#include "STB/stb_image.h"

Image::Image()	:
	m_iWidth(0),
	m_iHeight(0),
	m_pImage(NULL)
{
}

Image::~Image()
{
	close();
}

Image::Ptr Image::create(const std::string& _strFilename)
{
	INSTANCE(pImage, Image())

	if (false == pImage->initialize(_strFilename))
	{
		pImage.reset();
	}

	return	pImage;
}

bool Image::initialize(const std::string& _strFilename)
{
	delete[]	m_pImage;

	File::Ptr	pFile	= File::create();

	if (nullptr == pFile)
	{
		Log::instance()->logError("Unable to create file interface");

		return	false;
	}

	if (false == pFile->open(_strFilename, true))
	{
		Log::instance()->logError("Unable to open image %s", _strFilename.c_str());

		return	false;
	}

	int	iLength	= pFile->getLength();

	DEFINESHAREDBUFFER(pImage, uint8_t, iLength)

	if (false == pFile->readBuffer((void*)pImage.get(), iLength))
	{
		Log::instance()->logError("Unable to read image %s", _strFilename.c_str());

		return	false;
	}

	pFile->close();
	pFile.reset();

	int	iWidth;
	int	iHeight;
	int	iBPP;
	
	// Load image as a 32 bit texture
	m_pImage	= (uint32_t*)stbi_load_from_memory(pImage.get(), iLength, &iWidth, &iHeight, &iBPP, 4);

	if (NULL == m_pImage)
	{
		Log::instance()->logError("Unable to load image %s", _strFilename.c_str());

		return	false;
	}
	
	m_iWidth	= iWidth;
	m_iHeight	= iHeight;

#if (!defined __ANDROID__ && !defined ANGLE) || (!defined __ARMEL__ && defined __APPLE__)
	uint32_t*	pBuffer	= m_pImage;

	for (int iLoop = 0; iLoop < m_iWidth * m_iHeight; ++iLoop)
	{
		uint32_t	uValue	= *pBuffer;
		
		*pBuffer	= (uValue & 0xFF00FF00) | ((uValue & 0x00FF0000) >> 16) | ((uValue & 0x000000FF) << 16);

		pBuffer++;
	}
#endif

	return	true;
}

void Image::close()
{
	delete[]	m_pImage;

	m_pImage	= NULL;
}
