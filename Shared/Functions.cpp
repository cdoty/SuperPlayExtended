// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <algorithm>
#include <sstream>
#include <float.h>

#include "File.h"
#include "Functions.h"
#include "Log.h"
#include "PlatformDefines.h"
#include "UTF8/utf8.h"

std::random_device	Functions::ms_randomDevice;	// Random device

void Functions::convertRGBAToARGB(const uint32_t* _pSource, uint32_t* _pDestination, int _iPixels, bool _bTransparent)
{
	const uint32_t*	pSrc	= _pSource;
	uint32_t*		pDest	= _pDestination;

	// Convert from 32 RGBA to ARGB
	for (int iLoop = 0; iLoop < _iPixels; ++iLoop)
	{
		uint32_t	uPixel	= *pSrc;
		
		uint32_t	uAlpha	= (uPixel & 0xFF000000) >> 24;
		uint32_t	uBlue	= (uPixel & 0x00FF0000) >> 16;
		uint32_t	uGreen	= (uPixel & 0x0000FF00) >> 8;
		uint32_t	uRed	= (uPixel & 0x000000FF);

		if (true == _bTransparent && 0 == uAlpha)
		{
			*pDest	= 0x00000000;
		}

		else
		{
			*pDest	= (uAlpha << 24) | (uRed << 16) | (uGreen << 8) | uBlue;
		}

		pSrc++;
		pDest++;
	}
}

int Functions::getHash(const std::string& _strString)
{
	uint32_t	iHash	= 5381;
	
	size_t	t_c	= _strString.size();

	for (size_t iLoop = 0; iLoop < t_c; ++iLoop)
	{
		iHash	= ((iHash << 5) + iHash) + _strString[iLoop];
	}

	return	iHash;
}

uint16_t Functions::adjustForEndian(uint16_t _value)
{
	uint16_t	value;

	if (true == gsc_bSwapEndian)
	{
		uint8_t	byte1	= (_value & 0xFF000000) >> 24;
		uint8_t	byte2	= (_value & 0x00FF0000) >> 16;

		value	= ((uint16_t)byte2 << 8) | (uint16_t)byte1;
	}

	else
	{
		value	= _value;
	}

	return	value;
}

uint32_t Functions::adjustForEndian(uint32_t _value)
{
	uint32_t	value;

	if (true == gsc_bSwapEndian)
	{
		uint8_t	byte1	= (_value & 0xFF000000) >> 24;
		uint8_t	byte2	= (_value & 0x00FF0000) >> 16;
		uint8_t	byte3	= (_value & 0x0000FF00) >> 8;
		uint8_t	byte4	= (_value & 0x000000FF);

		value	= ((uint32_t)byte4 << 24) | ((uint32_t)byte3 << 16) | ((uint32_t)byte2 << 8) | (uint32_t)byte1;
	}

	else
	{
		value	= _value;
	}

	return	value;
}

void Functions::convertToLowerCase(std::string& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::tolower);
	}
}

void Functions::convertToLowerCase(std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::tolower);
	}
}

void Functions::convertToUpperCase(std::string& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::toupper);
	}
}

void Functions::convertToUpperCase(std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::toupper);
	}
}

int Functions::convertStringToInt(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	std::stoi(_strString);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertStringToInt: Error %s", _exception.what());
		}
	}

	return	0;
}

int Functions::convertStringToInt(const std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	std::stoi(_strString, NULL, 10);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertStringToInt: Error %s", _exception.what());
		}
	}

	return	0;
}

float Functions::convertStringToFloat(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	(float)(std::stod(_strString, NULL));
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertStringToFloat: Error %s", _exception.what());
		}
	}

	return	0.0f;
}

float Functions::convertStringToFloat(const std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	(float)(std::stod(_strString, NULL));
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertStringToFloat: Error %s", _exception.what());
		}
	}

	return	0.0f;
}

uint32_t Functions::convertHexStringToUnsigned32(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::string	strTemp;
			int			iBase = 16;

			if (_strString.find("0x") == _strString.npos && _strString.find("0X") == _strString.npos)
			{
				strTemp	= "0x";
			}

			strTemp	+= _strString;

			return	std::stoul(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertHexStringToUnsigned32: Error %s", _exception.what());
		}
	}

	return	0;
}

uint32_t Functions::convertHexStringToUnsigned32(const std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::wstring	strTemp;
			int				iBase = 16;

			if (_strString.find(L"0x") == _strString.npos && _strString.find(L"0X") == _strString.npos)
			{
				strTemp	= L"0x";
			}

			strTemp	+= _strString;

			return	std::stoul(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertHexStringToUnsigned32: Error %s", _exception.what());
		}
	}

	return	0;
}

uint64_t Functions::convertHexStringToUnsigned64(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::string	strTemp;
			int			iBase = 16;

			if (_strString.find("0x") == _strString.npos && _strString.find("0X") == _strString.npos)
			{
				strTemp	= "0x";
			}

			strTemp	+= _strString;

			return	std::stoull(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertHexStringToUnsigned64: Error %s", _exception.what());
		}
	}

	return	0;
}

uint64_t Functions::convertHexStringToUnsigned64(const std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::wstring	strTemp;
			int				iBase = 16;

			if (_strString.find(L"0x") == _strString.npos && _strString.find(L"0X") == _strString.npos)
			{
				strTemp	= L"0x";
			}

			strTemp	+= _strString;

			return	std::stoull(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertHexStringToUnsigned64: Error %s", _exception.what());
		}
	}

	return	0;
}

bool Functions::convertStringToBool(const std::string& _strString)
{
	std::string	strTemp;

	if (false == _strString.empty())
	{
		strTemp	= _strString;

		convertToLowerCase(strTemp);
	}

	return	"true" == strTemp;
}

bool Functions::convertStringToBool(const std::wstring& _strString)
{
	std::wstring	strTemp;

	if (false == _strString.empty())
	{
		strTemp	= _strString;

		convertToLowerCase(strTemp);
	}

	return	L"true" == strTemp;
}

std::string Functions::convertBoolToString(bool _bValue)
{
	return	true == _bValue ? "true" : "false";
}

std::wstring Functions::convertBoolToWideString(bool _bValue)
{
	return	true == _bValue ? L"true" : L"false";
}

void Functions::deleteString(std::string& _strSource, const std::string& _strDelete)
{
	if (false == _strSource.empty())
	{
		size_t	strPos	= _strSource.find(_strDelete);

		if (strPos != std::string::npos)
		{
			_strSource.erase(strPos, _strDelete.length());
		}
	}
}

void Functions::deleteString(std::wstring& _strSource, const std::wstring& _strDelete)
{
	if (false == _strSource.empty())
	{
		size_t	strPos	= _strSource.find(_strDelete);

		if (strPos != std::string::npos)
		{
			_strSource.erase(strPos, _strDelete.length());
		}
	}
}

void Functions::removeQuotes(std::string& _strSource)
{
	if (_strSource.length() > 2)
	{
		if ('\"' == _strSource[0])
		{
			_strSource.erase(0, 1);
		}

		int	iEnd	= (int)_strSource.length() - 1;

		if ('\"' == _strSource[iEnd])
		{
			_strSource.erase(iEnd, 1);
		}
	}
}

void Functions::removeQuotes(std::wstring& _strSource)
{
	if (_strSource.length() > 2)
	{
		if (L'\"' == _strSource[0])
		{
			_strSource.erase(0, 1);
		}

		int	iEnd	= (int)_strSource.length() - 1;

		if (L'\"' == _strSource[iEnd])
		{
			_strSource.erase(iEnd, 1);
		}
	}
}

std::wstring Functions::convertUTF8String(const std::string& _strString)
{
	std::wstring	strValue;

	if (false == _strString.empty())
	{
		utf8::utf8to16(_strString.begin(), _strString.end(), back_inserter(strValue));
	}

	return	strValue;
}

std::string Functions::convertToUTF8String(const std::wstring& _strString)
{
	std::string	strValue;

	if (false == _strString.empty())
	{
		utf8::utf16to8(_strString.begin(), _strString.end(), back_inserter(strValue));
	}

	return	strValue;
}

uint32_t Functions::getNextPowerOfTwo(uint32_t _value)
{
	_value--;

	_value	|=	_value >> 1;
	_value	|=	_value >> 2;
	_value	|=	_value >> 4;
	_value	|=	_value >> 8;
	_value	|=	_value >> 16;
	
	_value++;

	return 	_value;
}

float Functions::getRandomRange(float _fLower, float _fUpper)
{
	if (_fLower >= _fUpper)
	{
		return	_fLower;
	}

	std::uniform_real_distribution<float>	randomValue(_fLower, _fUpper);

	return	randomValue(ms_randomDevice);
}

float Functions::getRandomPercent()
{
	return	getRandomRange(0.0f, 1.0f);
}

int Functions::getRandomRange(int _iLower, int _iUpper)
{
	if (_iLower >= _iUpper)
	{
		return	_iLower;
	}

	std::uniform_int_distribution<int>	randomValue(_iLower, _iUpper);

	return	randomValue(ms_randomDevice);
}

std::vector<std::string> Functions::tokenize(const std::string& _strSource, char _delimiter)
{
	std::stringstream			stringStream(_strSource);
	std::string					strFound;
	std::vector<std::string>	result;

	while (std::getline(stringStream, strFound, _delimiter))
	{
		result.push_back(strFound);
	}
	
	return	result;
}

std::vector<std::wstring> Functions::tokenize(const std::wstring& _strSource, wchar_t _delimiter)
{
	std::wstringstream			stringStream(_strSource);
	std::wstring				strFound;
	std::vector<std::wstring>	result;

	while (std::getline(stringStream, strFound, _delimiter))
	{
		result.push_back(strFound);
	}
	
	return	result;
}

void Functions::createINIString(std::string& _strINI, const std::string& _strName, const std::string& _strValue, const std::string& _strPrefix)
{
	if (false == _strPrefix.empty())
	{
		_strINI	= _strPrefix;
		_strINI	+= _strName;
	}

	else
	{
		_strINI	= _strName;
	}

	_strINI	+= " = ";
	_strINI	+= _strValue;
	_strINI	+= "\n";
}

void Functions::createINIString(std::string& _strINI, const std::string& _strName, int _iValue, const std::string& _strPrefix)
{
	if (false == _strPrefix.empty())
	{
		_strINI	= _strPrefix;
		_strINI	+= _strName;
	}

	else
	{
		_strINI	= _strName;
	}

	_strINI	+= " = ";
	_strINI	+= std::to_string(_iValue);
	_strINI	+= "\n";
}

void Functions::createINIString(std::string& _strINI, const std::string& _strName, float _fValue, const std::string& _strPrefix, bool _bTruncate)
{
	if (false == _strPrefix.empty())
	{
		_strINI	= _strPrefix;
		_strINI	+= _strName;
	}

	else
	{
		_strINI	= _strName;
	}

	char	szString[32];

	if (true == _bTruncate)
	{
		sprintf(szString, "%0.2f", _fValue);
	}

	else
	{
		sprintf(szString, "%f", _fValue);
	}

	_strINI	+= " = ";
	_strINI	+= szString;
	_strINI	+= "\n";
}

void Functions::createINIString(std::string& _strINI, const std::string& _strName, bool _bValue, const std::string& _strPrefix)
{
	if (false == _strPrefix.empty())
	{
		_strINI	= _strPrefix;
		_strINI	+= _strName;
	}

	else
	{
		_strINI	= _strName;
	}

	_strINI	+= " = ";
	_strINI	+= (true == _bValue ? "true" : "false");
	_strINI	+= "\n";
}

float Functions::getNextLowest(float _fValue)
{
	float	fWholeNumber;

	float	fFraction	= modff(_fValue, &fWholeNumber);

	if (fFraction < FLT_EPSILON)
	{
		return	fWholeNumber - 1.0f;
	}

	return	fWholeNumber;
}

float Functions::getNextHighest(float _fValue)
{
	float	fWholeNumber;

	modff(_fValue, &fWholeNumber);

	return	fWholeNumber + 1.0f;
}

float Functions::easeIn(float _fTime)
{
	return	_fTime * _fTime;
}

float Functions::easeOut(float _fTime)
{
	float	fValue	= 1 - _fTime;

	fValue	*= fValue;

	return	1.0f - fValue;
}

float Functions::easeInOut(float _fTime)
{
	return	 _fTime * _fTime * (3.0f - 2.0f * _fTime);
}

