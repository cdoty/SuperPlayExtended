// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stdarg.h>

#include "File.h"
#include "Log.h"
#include "SystemFunctions.h"

Log::Ptr	Log::m_pInstance	= nullptr;	// Instance

#ifdef WRITE_LOGS
static const char*	gsc_szDefaultLogFile	= "Log.html";	// Default log file
static const char*	gsc_szDefaultLogTitle	= "Log Output";	// Defauly log title
#endif

Log::Log()
{
	createLog();
}

Log::~Log()
{
	closeLog();
}

Log::Ptr Log::instance()
{
	if (nullptr == m_pInstance)
	{
		DEFINEINSTANCE(m_pInstance, Log());
	}
	
	return	m_pInstance;	
}

void Log::reset()
{
	if (m_pInstance != nullptr)
	{
		m_pInstance.reset();
	}
}

void Log::logMessage(const char* _szFormat, ...)
{
	va_list	args;
	char	szString[8192];

	va_start(args, _szFormat);

	vsprintf(szString, _szFormat, args);

	va_end(args);

	logString(LogMessage, szString);
}

void Log::logWarning(const char* _szFormat, ...)
{
	va_list	args;
	char	szString[8192];

	va_start(args, _szFormat);

	vsprintf(szString, _szFormat, args);

	va_end(args);

	logString(LogWarning, szString);
}

void Log::logError(const char* _szFormat, ...)
{
	static char	szString[8192];

	va_list	args;

	va_start(args, _szFormat);

	vsprintf(szString, _szFormat, args);

	va_end(args);

	logString(LogError, szString);
}

void Log::log(eLogType _eType, const char* _szFormat, ...)
{
	va_list	args;
	char	szString[8192];

	va_start(args, _szFormat);

	vsprintf(szString, _szFormat, args);

	va_end(args);

	logString(_eType, szString);
}

void Log::createLog()
{
#ifdef WRITE_LOGS
	// Erase any existing log.
	File::deleteFile(gsc_szDefaultLogFile);

	writeHTMLStart();
#endif

	log(LogMessage, "Logging started");
}

void Log::closeLog()
{
	log(LogMessage, "Logging ended");
	
#ifdef WRITE_LOGS
	writeHTMLEnd();

	// Log messages to log file.
	File::Ptr	pFile	= File::create();

	if (pFile != nullptr)
	{
		if (true == pFile->create(gsc_szDefaultLogFile))
		{
			size_t	t_c	= m_strLogStrings.size();
		
			for (size_t iLoop = 0; iLoop < t_c; ++iLoop)
			{
				pFile->writeString(m_strLogStrings[iLoop]);
			}
		}

		pFile->close();
	}
#endif
}

void Log::logString(eLogType _eType, const std::string& _strNewString)
{
#ifdef WRITE_LOGS
	switch (_eType)
	{
		case LogError:
			m_strLogStrings.push_back("<FONT COLOR=#FF0000>Error: ");

			break;

		case LogWarning:
			m_strLogStrings.push_back("<FONT COLOR=#FFFF00>Warning: ");

			break;

		case LogMessage:
		default:
			m_strLogStrings.push_back("<FONT COLOR=#00FF00>");

			break;
	}
#endif

	m_strLogStrings.push_back(_strNewString);
	m_strLogStrings.push_back("</FONT><BR>");

	debugOutput(_strNewString.c_str());
	debugOutput("\n");
}

#ifdef WRITE_LOGS
void Log::writeHTMLStart()
{
	m_strLogStrings.push_back("<HTML><HEAD><TITLE>");
	m_strLogStrings.push_back(gsc_szDefaultLogTitle);
	m_strLogStrings.push_back("</TITLE></HEAD><BODY BGCOLOR=#00001F>");
}

void Log::writeHTMLEnd()
{
	m_strLogStrings.push_back("</BODY></HTML>\n");
}
#endif
