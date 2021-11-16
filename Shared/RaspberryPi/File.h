#pragma once

#include "Macros.h"
#include "FileBase.h"

class File : public FileBase
{
	public:
		PTR(File)
		SUPER(FileBase)

		// Create
		static Ptr create();

		// Create file
		bool create(const std::string& _strFilename, bool _bBinary = false);
	
		// Open file
		bool open(const std::string& _strFilename, bool _bBinary = false) override;
	
		// Write buffer
		bool writeBuffer(const uint8_t* _pBuffer, int iBufferSize) const;
		
		// Write string
		bool writeString(const std::string& _strString) const;
		
		// Delete file
		static bool deleteFile(const std::string& _strFilename);
		
	private:
		// Constructor
		File();
};
