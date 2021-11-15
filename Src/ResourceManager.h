#pragma once

#include "Image.h"
#include "Macros.h"

class ResourceManager
{
	public:
		PTR(ResourceManager)

		// Destructor
		~ResourceManager();

		// Create resource manager
		static Ptr create();

		// Initialize
		bool initialize();

		// Close
		void close();

		// Create image
		int createImage(const std::string& _strFilename);

		// Get image
		Image::Ptr getImage(int _iHash) const;

		// Remove image
		void removeImage(int _iHash);

	private:
		struct ImageData
		{
			Image::Ptr	pImage;				// Image
			int			iReferenceCount;	// Reference count
		
			ImageData() : iReferenceCount(0) {}
		};

		typedef	std::unordered_map<int, ImageData> ImageMap;

		ImageMap	m_mapImages;	// Images

		// Constructor
		ResourceManager();
};
