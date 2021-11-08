#pragma once

#include <string>

class FileBase
{
	public:
		enum SeekPoint
		{
			SeekFromStart	= 0,
			SeekFromCurrent,
			SeekFromEnd
		};

		// Constructor
		FileBase();

		// Destructor
		~FileBase();

		// Open file
		bool open(const std::string& _strFilename, bool _bBinary = false);
	
		// Close file
		bool close();
		
		// Read 8 bit value
		bool read8Bit(char& _iValue) const;
		
		// Read unsigned 8 bit value
		bool readUnsigned8Bit(uint8_t& _iValue) const;
		
		// Read 16 bit
		bool read16Bit(short& _iValue) const;
		
		// Read unsigned 16 bit
		bool readUnsigned16Bit(uint16_t& _iValue) const;
		
		// Read 32 bit
		bool read32Bit(int& _iValue) const;
		
		// Read unsigned 32 bit
		bool readUnsigned32Bit(uint32_t& _iValue) const;
		
		// Read 64 bit
		bool read64Bit(int64_t& _iValue) const;
		
		// Read unsigned 64 bit
		bool readUnsigned64Bit(uint64_t& _iValue) const;
		
		// Read null terminated string into std::string
		bool readString(std::string& _strValue);

		// Read line
		bool readLine(std::string& _strLine, int _iMaxSize = 8192) const;
		bool readLineUTF8(std::string& _strLine, int _iMaxSize = 8192) const;

		// Read buffer
		bool readBuffer(void* _pBuffer, int _iBufferSize, int _iElementSize = 1) const;
		bool readBuffer(uint8_t** _pBuffer, int _iBufferSize, int _iElementSize = 1) const;

		// Seek
		bool seek(int _iOffset, SeekPoint _eSeekPoint = SeekFromCurrent) const;
		
		// Get position
		int getPosition() const;
		
		// Get length
		int getLength() const;
		
		// End of file?
		bool endOfFile() const;

		// Get handle
		FILE* getHandle() const {return m_pHandle;}

	private:
		FILE*	m_pHandle;	// File handle
		bool	m_bBinary;	// Writing in binary format?
};
