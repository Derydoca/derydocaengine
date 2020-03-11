#include "EnginePch.h"
#include "Utilities\TexturePacker.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include "Rendering\Texture.h"

namespace DerydocaEngine::Utilities
{

	// Method for sorting images
	bool compareTexturePackerImageBySize(TexturePackerImage lhs, TexturePackerImage rhs)
	{
		return lhs.size.x > rhs.size.x;
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
		const unsigned long id,
		const float2& bearing,
		const float2& advance,
		const unsigned char* imageBuffer,
		const int2& size,
		const int channels)
	{
		// Store the image
		TexturePackerImage image(id, channels, size, bearing, advance);
		m_images.push_back(image);

		// Store the image data in the image buffers object
		unsigned char* buffer = m_imageBuffers[id];
		delete[] buffer;
		int imageBufferSize = size.x * size.y * channels;
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
				int2 size = image.size;
				totalGlyphArea += size.x * size.y;
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
			int2 imageSize = m_images[i].size;
			int imageWidth = imageSize.x;
			int imageHeight = imageSize.y;

			IntRect imgRect;

			assert(imageWidth <= textureSheetWidth);

			bool found = false;

			// Find a place for the image by progressively looking farther down the image after scanning the current line
			for (int scanPosY = 0; scanPosY < textureSheetWidth * 2 && !found; scanPosY++)
			{
				imgRect.y = scanPosY;
				imgRect.dy = scanPosY + imageHeight;

				// Scan through each X location in the line
				for (int scanPosX = 0; scanPosX <= textureSheetWidth - imageWidth && !found; scanPosX++)
				{
					imgRect.x = scanPosX;
					imgRect.dx = scanPosX + imageWidth;

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
						m_packedImageData.AddImage(scanPosX, scanPosY, m_images[i], m_imageBuffers[m_images[i].id]);

						// Remember where this is located at
						imageBounds.push_back(IntRect(scanPosX, scanPosY, scanPosX + imageWidth, scanPosY + imageHeight));

						// Store the location of the image so that we can calculate the normalized position
						imageLocations[i] = IntRect(scanPosX, scanPosY, scanPosX + imageWidth, scanPosY + imageHeight);

						found = true;
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
			float x = (float)loc.x / m_packedImageData.getWidth();
			float y = (float)loc.y / m_packedImageData.getHeight();
			float dx = (float)loc.dx / m_packedImageData.getWidth();
			float dy = (float)loc.dy / m_packedImageData.getHeight();
			m_images[i].imageRect = { x, y, dx, dy };
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

	int TexturePacker::getIntersectingImageXAdvance(const std::vector<IntRect>& imageBounds, const IntRect& rect)
	{
		// For each image boundary
		for (auto imageBoundary : imageBounds)
		{
			// If it is overlapping with the rectangle
			if (IntRect::IsRectOverlapping(imageBoundary, rect))
			{
				// Return the number of pixels we can advance to be in front of this intersecting image boundary
				return imageBoundary.dx - rect.x;
			}
		}

		// If no intersections were found, return an invalid value so it can be tested against
		return -1;
	}

}
