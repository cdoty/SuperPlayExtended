#pragma once

#include "FileBase.h"
#include "Macros.h"

class File : public FileBase
{
	public:
		PTR(File)
		SUPER(FileBase)

		// Create
		static Ptr create();

		// Create file
		bool create(const std::string& _strFilename, bool _bBinary = false);
	
		// Append file
		bool append(const std::string& _strFilename, bool _bBinary = false);
	
		// Write signed/unsigned 8 bit value
		bool write8Bit(uint8_t _iValue) const;
		
		// Write signed/unsigned 32 bit value
		bool write32Bit(int _iValue) const;
		
		// Write signed/unsigned 64 bit value
		bool write64Bit(int64_t _iValue) const;
		
		// Write float value
		bool writeFloat(float _fValue) const;
		
		// Write line
		bool writeLine(const std::string& _strLine) const;

		// Write buffer
		bool writeBuffer(const uint8_t* _pBuffer, int iBufferSize) const;
		
		// Write string
		bool writeString(const std::string& _strString) const;
		bool writeStringUTF8(const std::wstring& _strString) const;
		
		// Delete file
		static bool deleteFile(const std::string& _strFilename);
		
		// Get handle
		FILE* getHandle() const {return m_pHandle;}

	private:
		FILE*	m_pHandle;	// File handle
		bool	m_bBinary;	// Writing in binary format?

		// Constructor
		File();
};
