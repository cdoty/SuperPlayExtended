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
