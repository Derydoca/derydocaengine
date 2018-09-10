#include "TexturePacker.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include "Texture.h"

// Method for sorting images
bool compareTexturePackerImageBySize(TexturePackerImage lhs, TexturePackerImage rhs)
{
	return lhs.getWidth() > rhs.getWidth();
}

TexturePacker::TexturePacker(int const& channels)
{
	assert(channels > 0);
	m_packedImageData = TexturePackerTextureData(channels);
	m_isDirty = false;
}

TexturePacker::~TexturePacker()
{
	freeSubImageData();
}

void TexturePacker::addImage(
	unsigned long const& id,
	float const& sizeX, 
	float const& sizeY, 
	float const& bearingX, 
	float const& bearingY, 
	float const& advanceX, 
	float const& advanceY, 
	unsigned char* const& imageBuffer,
	int const& width, 
	int const& height, 
	int const& channels)
{
	// Store the image
	TexturePackerImage image(id, width, height, channels, sizeX, sizeY, bearingX, bearingY, advanceX, advanceY);
	m_images.push_back(image);

	// Store the image data in the image buffers object
	unsigned char* buffer = m_imageBuffers[id];
	delete[] buffer;
	int imageBufferSize = width * height * channels;
	unsigned char* newBuffer = new unsigned char[imageBufferSize];
	for (int i = 0; i < imageBufferSize; i++)
	{
		newBuffer[i] = imageBuffer[i];
	}
	m_imageBuffers[id] = newBuffer;

	// Note that the texture packer data is dirty
	m_isDirty = true;
}

void TexturePacker::packImages()
{
	// Only re-pack the images if the texture packer is dirty
	if (!m_isDirty)
	{
		return;
	}

	// Store a vector of rectangles representing the location and size of images that have been placed
	std::vector<IntRect> imageBounds;

	// Figure out an image width that should contain all glyphs without issue
	int textureSheetWidth = 128;
	{
		float totalGlyphArea = 0.0f;
		for (auto image : m_images)
		{
			totalGlyphArea += (float)image.getWidth() * image.getHeight();
		}
		while (textureSheetWidth * textureSheetWidth < totalGlyphArea)
		{
			textureSheetWidth = textureSheetWidth * 2;
		}
	}

	// Clear the packed image data
	m_packedImageData.Resize(textureSheetWidth, 1);

	// Sort all of the images
	sort(m_images.begin(), m_images.end(), compareTexturePackerImageBySize);

	// Create a temporary image location array
	IntRect* imageLocations = new IntRect[m_images.size()];

	// For each image
	for (size_t i = 0; i < m_images.size(); i++)
	{
		int imageWidth = m_images[i].getWidth();
		int imageHeight = m_images[i].getHeight();

		IntRect imgRect;

		assert(imageWidth <= textureSheetWidth);

		bool found = false;

		// Find a place for the image by progressively looking farther down the image after scanning the current line
		for (int scanPosY = 0; scanPosY < textureSheetWidth * 2 && !found; scanPosY++)
		{
			imgRect.setY(scanPosY);
			imgRect.setDy(scanPosY + imageHeight);

			// Scan through each X location in the line
			for (int scanPosX = 0; scanPosX <= textureSheetWidth - imageWidth && !found; scanPosX++)
			{
				imgRect.setX(scanPosX);
				imgRect.setDx(scanPosX + imageWidth);

				// This x,y location is valid if there is nothing obstructing the placement
				int spaceToSkip = getIntersectingImageXAdvance(imageBounds, imgRect);

				// If it is valid
				if (spaceToSkip < 0)
				{
					// If the image placed at this position will exceede the size of the image, resize it so there is room to fit
					if (scanPosY + imageHeight > m_packedImageData.getHeight())
					{
						m_packedImageData.Resize(textureSheetWidth, scanPosY + imageHeight);
					}

					// Add the image
					m_packedImageData.AddImage(scanPosX, scanPosY, &m_images[i], m_imageBuffers[m_images[i].getID()]);

					// Remember where this is located at
					imageBounds.push_back(IntRect(scanPosX, scanPosY, scanPosX + imageWidth, scanPosY + imageHeight));

					// Store the location of the image so that we can calculate the normalized position
					imageLocations[i] = IntRect(scanPosX, scanPosY, scanPosX + imageWidth, scanPosY + imageHeight);

					found = true;
					std::cout << "Placed image " << i + 1 << "/" << m_images.size() << "\n";
				}
				else
				{
					scanPosX += spaceToSkip - 1;
				}
			}
		}
	}

	// Convert the image positions to normalized values and store it with the image data
	for (size_t i = 0; i < m_images.size(); i++)
	{
		IntRect loc = imageLocations[i];
		float x = (float)loc.getX() / m_packedImageData.getWidth();
		float y = (float)loc.getY() / m_packedImageData.getHeight();
		float dx = (float)loc.getDx() / m_packedImageData.getWidth();
		float dy = (float)loc.getDy() / m_packedImageData.getHeight();
		m_images[i].setTextureSheetRectangle(x, y, dx, dy);
	}

	// Delete the temporary image locations array
	delete[] imageLocations;

	freeSubImageData();
}

void TexturePacker::freeSubImageData()
{
	// Delete all image buffer data
	for (auto imageBuffer : m_imageBuffers)
	{
		delete[] imageBuffer.second;
	}
	m_imageBuffers.clear();
}

int TexturePacker::getIntersectingImageXAdvance(std::vector<IntRect> imageBounds, IntRect rect)
{
	// For each image boundary
	for (auto imageBoundary : imageBounds)
	{
		// If it is overlapping with the rectangle
		if (IntRect::IsRectOverlapping(imageBoundary, rect))
		{
			// Return the number of pixels we can advance to be in front of this intersecting image boundary
			return imageBoundary.getDx() - rect.getX();
		}
	}

	// If no intersections were found, return an invalid value so it can be tested against
	return -1;
}
