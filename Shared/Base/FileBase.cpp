// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FileBase.h"
#include "Functions.h"
#include "Log.h"

FileBase::FileBase() :
	m_pHandle(NULL),
	m_bBinary(false)
{
}

FileBase::~FileBase()
{
	close();
}

bool FileBase::open(const std::string& _strFilename, bool _bBinary)
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

bool FileBase::close()
{
	if (m_pHandle != NULL)
	{
		fclose(m_pHandle);
	
		m_pHandle	= NULL;
	}
	
	return	true;
}

bool FileBase::read8Bit(char& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(char), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool FileBase::readUnsigned8Bit(uint8_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(uint8_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool FileBase::read16Bit(short& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(short), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool FileBase::readUnsigned16Bit(uint16_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(uint16_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool FileBase::read32Bit(int& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(int), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool FileBase::readUnsigned32Bit(uint32_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(uint32_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool FileBase::read64Bit(int64_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(int64_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool FileBase::readUnsigned64Bit(uint64_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(uint64_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool FileBase::readString(std::string& _strValue)
{
	_strValue.clear();

	while (true)
	{
		char	value;

		if (false == read8Bit(value))
		{
			return	false;
		}
		
		if (0 == value)
		{
			break;
		}

		_strValue	+= value;
	}

	return	true;
}

bool FileBase::readLine(std::string& _strLine, int _iMaxSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	_strLine.clear();

	DEFINESHAREDBUFFER(pBuffer, char, _iMaxSize);
							
	if (nullptr == pBuffer || NULL == fgets(pBuffer.get(), _iMaxSize, m_pHandle))
	{
		return	false;
	}

	_strLine	= pBuffer.get();

	size_t	found	= _strLine.find('\n');
	
	if (found != std::string::npos)
	{
		_strLine.erase(found, 1);
	}

	return	true;
}

bool FileBase::readLineUTF8(std::string& _strLine, int _iMaxSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	_strLine.clear();

	DEFINESHAREDBUFFER(pBuffer, wchar_t, _iMaxSize);
							
	if (nullptr == pBuffer || NULL == fgetws(pBuffer.get(), _iMaxSize, m_pHandle))
	{
		return	false;
	}

	std::wstring	strBuffer	= pBuffer.get();
	std::string		strValue	= Functions::convertToUTF8String(strBuffer);

	_strLine	= strValue;

	size_t	found	= _strLine.find('\n');
	
	if (found != std::string::npos)
	{
		_strLine.erase(found, 1);
	}

	return	true;
}

bool FileBase::readBuffer(void* _pBuffer, int _iBufferSize, int _iElementSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(_pBuffer, (size_t)_iElementSize, (size_t)_iBufferSize, m_pHandle) != (size_t)_iBufferSize)
	{
		return	false;
	}
	
	return	true;
}

bool FileBase::readBuffer(uint8_t** _pBuffer, int _iBufferSize, int _iElementSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (NULL == *_pBuffer)
	{
		*_pBuffer	= new (std::nothrow) uint8_t[(size_t)_iBufferSize];

		if (nullptr == *_pBuffer)
		{
			return	false;
		}
	}
	
	return	readBuffer(*_pBuffer, _iBufferSize, _iElementSize);
}

bool FileBase::seek(int _iOffset, SeekPoint _eSeekPoint) const
{
	int	iOrigin;
	
	switch (_eSeekPoint)
	{
		case SeekFromStart:
			iOrigin	= SEEK_SET;
			
			break;
			
		case SeekFromCurrent:
			iOrigin	= SEEK_CUR;
			
			break;
			
		case SeekFromEnd:
			iOrigin	= SEEK_END;
			
			break;
	
		default:
			return	false;
	}
	
	if (fseek(m_pHandle, _iOffset, iOrigin) != 0)
	{
		return	false;
	}
	
	return	true;
}

int FileBase::getPosition() const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	0;
	}

	return	ftell(m_pHandle);
}

int FileBase::getLength() const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	0;
	}

	int	iCurrentPosition	= ftell(m_pHandle);
	
	if (-1 == iCurrentPosition || fseek(m_pHandle, 0, SEEK_END) != 0)
	{
		return	-1;
	}
	
	int	iLength	= ftell(m_pHandle);
	
	fseek(m_pHandle, iCurrentPosition, SEEK_SET);
	
	return	iLength;
}

bool FileBase::endOfFile() const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	return	feof(m_pHandle) != 0;
}
