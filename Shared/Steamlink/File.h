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

		// Open file
		bool open(const std::string& _strFilename, bool _bBinary = false) override;
	
	private:
		// Constructor
		File();
};
