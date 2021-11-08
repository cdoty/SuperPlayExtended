#pragma once

#include "Image.h"
#include "Macros.h"

class ResourceManager
{
	public:
		PTR(ResourceManager)

		struct StoredImage
		{
			Image::Ptr	pImage;	// Image
			int			iHash;	// Name hash
		};

		// Destructor
		~ResourceManager();

		// Create resource manager
		static Ptr create();

		// Initialize
		bool initialize();

		// Load image
		int loadImage(const std::string& _strFilename);

		// Release image
		void	releaseImage(int _iIndex);

		// Get image
		Image::Ptr getImage(int _iIndex) const;

	private:
		std::vector<StoredImage>	m_vecImages;	// Images

		// Find image
		int findImage(int _iHash) const;

		// Add image
		int addImage(StoredImage& _storedImage);

		// Remove images
		void removeImages();

		// Constructor
		ResourceManager();
};
