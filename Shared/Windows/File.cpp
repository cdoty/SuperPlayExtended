// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "File.h"
#include "Functions.h"
#include "Log.h"

File::File() :
	m_pHandle(NULL),
	m_bBinary(false)
{
}

File::~File()
{
	close();
}

File::Ptr File::create()
{
	INSTANCE(pFile, File());

	return	pFile;
}

bool File::open(const std::string& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	fopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? "rb" : "rt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::open(const std::wstring& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	_wfopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? L"rb" : L"rt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::create(const std::string& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	fopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? "wb" : "wt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::create(const std::wstring& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	_wfopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? L"wb" : L"wt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::append(const std::string& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	fopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? "ab" : "at");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::append(const std::wstring& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	_wfopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? L"ab" : L"at");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::close()
{
	if (m_pHandle != NULL)
	{
		fclose(m_pHandle);
	
		m_pHandle	= NULL;
	}
	
	return	true;
}

bool File::read8Bit(char& _iValue) const
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

bool File::readUnsigned8Bit(uint8_t& _iValue) const
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

bool File::read16Bit(short& _iValue) const
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

bool File::readUnsigned16Bit(uint16_t& _iValue) const
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

bool File::read32Bit(int& _iValue) const
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

bool File::readUnsigned32Bit(uint32_t& _iValue) const
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

bool File::read64Bit(int64_t& _iValue) const
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

bool File::readUnsigned64Bit(uint64_t& _iValue) const
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

bool File::readString(std::string& _strValue)
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

bool File::readString(std::wstring& _strValue)
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

bool File::readLine(std::string& _strLine, int _iMaxSize) const
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

bool File::readLine(std::wstring& _strLine, int _iMaxSize) const
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

	_strLine	= pBuffer.get();

	size_t	found	= _strLine.find(L'\n');
	
	if (found != std::wstring::npos)
	{
		_strLine.erase(found, 1);
	}

	return	true;
}

bool File::readLineUTF8(std::string& _strLine, int _iMaxSize) const
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

bool File::readBuffer(void* _pBuffer, int _iBufferSize, int _iElementSize) const
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

bool File::readBuffer(uint8_t** _pBuffer, int _iBufferSize, int _iElementSize) const
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

bool File::write8Bit(uint8_t _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(&_iValue, sizeof(uint8_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::write32Bit(int _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(&_iValue, sizeof(int), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::write64Bit(int64_t _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(&_iValue, sizeof(int64_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::writeFloat(float _fValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(&_fValue, sizeof(float), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
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

bool File::writeLine(const std::string& _strLine) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (false == writeString(_strLine))
	{
		return	false;
	}

	if (false == writeString("\n"))
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

bool File::writeString(const std::wstring& _strString) const
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
		
		iBufferSize	*= sizeof(wchar_t);

		return	writeBuffer((const uint8_t*)_strString.c_str(), (int)iBufferSize);
	}
	
	return	true;
}

bool File::writeStringUTF8(const std::wstring& _strString) const
{
	if (false == _strString.empty())
	{
		std::string	strValue	= Functions::convertToUTF8String(_strString);

		size_t	iBufferSize;
		
		if (true == m_bBinary)
		{
			iBufferSize	= strValue.length() + 1;
		}
		
		else
		{
			iBufferSize	= strValue.length();
		}
		
		return	writeBuffer((const uint8_t*)strValue.c_str(), (int)iBufferSize);
	}
	
	return	true;
}

bool File::seek(int _iOffset, SeekPoint _eSeekPoint) const
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

int File::getPosition() const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	0;
	}

	return	ftell(m_pHandle);
}

int File::getLength() const
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

bool File::endOfFile() const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	return	feof(m_pHandle) != 0;
}

bool File::deleteFile(const std::string& _strFilename)
{
	if (remove(_strFilename.c_str()) != 0)
	{
		return	false;
	}
	
	return	true;
}

bool File::deleteFile(const std::wstring& _strFilename)
{
	if (_wremove(_strFilename.c_str()) != 0)
	{
		return	false;
	}
	
	return	true;
}
