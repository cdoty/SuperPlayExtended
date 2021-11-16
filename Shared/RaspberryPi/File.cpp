// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "File.h"
#include "Functions.h"
#include "Log.h"

File::File() :
	super()
{
}

File::Ptr File::create()
{
	INSTANCE(pFile, File());

	return	pFile;
}

bool File::create(const std::string& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	m_pHandle	= fopen(_strFilename.c_str(), true == _bBinary ? "wb" : "wt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::open(const std::string& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	m_pHandle	= fopen(_strFilename.c_str(), true == _bBinary ? "rb" : "rt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::writeBuffer(const uint8_t* _pBuffer, int iBufferSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(_pBuffer, 1, (size_t)iBufferSize, m_pHandle) != (size_t)iBufferSize)
	{
		return	false;
	}
	
	return	true;
}

bool File::writeString(const std::string& _strString) const
{
	if (false == _strString.empty())
	{
		size_t	iBufferSize;
		
		if (true == m_bBinary)
		{
			iBufferSize	= _strString.length() + 1;
		}
		
		else
		{
			iBufferSize	= _strString.length();
		}
		
		return	writeBuffer((const uint8_t*)_strString.c_str(), (int)iBufferSize);
	}
	
	return	true;
}

bool File::deleteFile(const std::string& _strFilename)
{
	if (remove(_strFilename.c_str()) != 0)
	{
		return	false;
	}
	
	return	true;
}
