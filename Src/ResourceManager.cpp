// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Functions.h"
#include "Log.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	removeImages();
}

ResourceManager::Ptr ResourceManager::create()
{
	INSTANCE(pResourceManager, ResourceManager())

	if (false == pResourceManager->initialize())
	{
		pResourceManager.reset();
	}

	return	pResourceManager;
}

bool ResourceManager::initialize()
{
	return	true;
}

int ResourceManager::loadImage(const std::string& _strFilename)
{
	int	iHash	= Functions::getHash(_strFilename);

	int	iImageIndex	= findImage(iHash);

	if (iImageIndex != -1)
	{
		return	iImageIndex;
	}

	Image::Ptr	pImage	= Image::create(_strFilename);

	if (nullptr == pImage)
	{
		return	-1;
	}

	StoredImage	storedImage;

	storedImage.pImage	= pImage;
	storedImage.iHash	= iHash;

	iImageIndex	= addImage(storedImage);

	return	iImageIndex;
}

void ResourceManager::releaseImage(int _iIndex)
{
	if (_iIndex >= 0 && _iIndex < (int)m_vecImages.size())
	{
		m_vecImages[_iIndex].iHash	= 0;
		m_vecImages[_iIndex].pImage.reset();
	}
}

Image::Ptr ResourceManager::getImage(int _iIndex) const
{
	if (_iIndex >= 0 && _iIndex < (int)m_vecImages.size())
	{
		return	m_vecImages[_iIndex].pImage;
	}

	return	nullptr;
}

int ResourceManager::findImage(int _iHash) const
{
	int	t_c	= (int)m_vecImages.size();

	for (int iLoop = 0; iLoop < t_c; ++iLoop)
	{
		if (m_vecImages[iLoop].iHash == _iHash)
		{
			return	iLoop;
		}
	}

	return	-1;
}

int ResourceManager::addImage(StoredImage& _storedImage)
{
	int	t_c	= m_vecImages.size();
	
	for (int iLoop = 0; iLoop < t_c; ++iLoop)
	{
		const StoredImage& pStoredImage	= m_vecImages[iLoop];

		if (0 == pStoredImage.iHash && nullptr == pStoredImage.pImage)
		{
			m_vecImages[iLoop]	= _storedImage;

			return	iLoop;
		}
	}

	m_vecImages.push_back(_storedImage);

	return	t_c;
}

void ResourceManager::removeImages()
{
	m_vecImages.clear();
}
