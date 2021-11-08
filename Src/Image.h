#pragma once

#include "Macros.h"

class Image
{
	public:
		PTR(Image)

		// Destructor
		~Image();

		// Create an image from a bitmap
		static Ptr create(const std::string& _strFilename);

		// Initialize
		bool initialize(const std::string& _strFilename);

		// Close
		void close();

		// Get pixels
		uint32_t* getPixels() const {return m_pImage;}

		// Get width
		int getWidth() const {return m_iWidth;}

		// Get height
		int getHeight() const {return m_iHeight;}

	private:
		int			m_iWidth;	// Image size
		int			m_iHeight;
		uint32_t*	m_pImage;	// Image pointer

		// Constructor
		Image();
};
