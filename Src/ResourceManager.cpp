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
	close();
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

void ResourceManager::close()
{
}

int ResourceManager::createImage(const std::string& _strFilename)
{
	int	iHash	= Functions::getHash(_strFilename);

	if (m_mapImages.find(iHash) == m_mapImages.end())
	{
		Image::Ptr	pImage	= Image::create(_strFilename);

		if (nullptr == pImage)
		{
			return	-1;
		}

		ImageData	imageData;

		imageData.pImage	= pImage;

		m_mapImages[iHash]	= imageData;
	}

	m_mapImages[iHash].iReferenceCount++;

	return	iHash;
}

Image::Ptr ResourceManager::getImage(int _iHash) const
{
	ImageMap::const_iterator	iterator	= m_mapImages.find(_iHash);

	if (iterator == m_mapImages.end())
	{
		return	nullptr;
	}
	
	return	iterator->second.pImage;
}

void ResourceManager::removeImage(int _iHash)
{
	ImageMap::const_iterator	iterator	= m_mapImages.find(_iHash);

	if (iterator != m_mapImages.end())
	{
		m_mapImages[_iHash].iReferenceCount--;
		
		if (m_mapImages[_iHash].iReferenceCount <= 0)
		{
			m_mapImages[_iHash].pImage.reset();
			
			m_mapImages.erase(iterator);
		}
	}
}
