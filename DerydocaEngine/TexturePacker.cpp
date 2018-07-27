#include "TexturePacker.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include "Texture.h"

TexturePacker::TexturePacker()
{
	m_isDirty = false;
}

TexturePacker::~TexturePacker()
{
	freeSubImageData();
}

void TexturePacker::addImage(int id, int width, int height, int bearingX, int bearyingY, int advanceX, int advanceY, unsigned char* imageBuffer)
{
	TexturePackerImage image(id, width, height, 1, bearingX, bearyingY, advanceX, advanceY);
	m_images.push_back(image);
	m_isDirty = true;
	unsigned char* buffer = m_imageBuffers[id];
	delete[] buffer;
	int bufferSize = width * height;
	unsigned char* newBuffer = new unsigned char[bufferSize];
	for (int i = 0; i < bufferSize; i++)
	{
		newBuffer[i] = imageBuffer[i];
	}
	m_imageBuffers[id] = newBuffer;
}

bool compareTexturePackerImageBySize(TexturePackerImage lhs, TexturePackerImage rhs)
{
	return lhs.getWidth() * lhs.getHeight() > rhs.getWidth() * rhs.getHeight();
}

void TexturePacker::packImages()
{
	// Only re-pack the images if the texture packer is dirty
	if (!m_isDirty)
	{
		return;
	}

	m_imageBounds.clear();
	dest.Resize(IMAGE_SIZE, 1);

	sort(m_images.begin(), m_images.end(), compareTexturePackerImageBySize);

	for (size_t i = 0; i < m_images.size(); i++)
	{
		int imageWidth = m_images[i].getWidth();
		int imageHeight = m_images[i].getHeight();

		assert(imageWidth <= IMAGE_SIZE);

		bool found = false;

		for (int scanPosY = 0; scanPosY < 2048 && !found; scanPosY++)
		{
			if (scanPosY + imageHeight > dest.getHeight())
			{
				dest.Resize(IMAGE_SIZE, scanPosY + imageHeight);
			}

			for (int scanPosX = 0; scanPosX < IMAGE_SIZE - m_images[i].getWidth() && !found; scanPosX++)
			{
				bool valid = isPointFree(scanPosX, scanPosY) && isRectFree(scanPosX, scanPosY, imageWidth, imageHeight);

				if (valid)
				{
					dest.AddImage(scanPosX, scanPosY, &m_images[i], m_imageBuffers[m_images[i].getID()]);
					m_imageBounds.push_back(IntRectangle(scanPosX, scanPosY, imageWidth, imageHeight));

					m_images[i].setTextureSheetRectangle(scanPosX, scanPosY, imageWidth, imageHeight);

					found = true;
					cout << "Placed image " << i + 1 << "/" << m_images.size() << endl;
				}
			}
		}
	}

	freeSubImageData();
}

void TexturePacker::freeSubImageData()
{
	cout << "FIX THE FREE SUB IMAGE DATA FUNCTION" << endl;
	/*for (auto imageBuffer : m_imageBuffers)
	{
		delete[] imageBuffer.second;
	}
	m_imageBuffers.empty();*/
}

bool TexturePacker::isPointFree(int x, int y)
{
	for (auto rect : m_imageBounds)
	{
		if (IntRectangle::IsPointInRect(rect, x, y))
		{
			return false;
		}
	}
	return true;
}

bool TexturePacker::isRectFree(int x, int y, int w, int h)
{
	for (auto rect : m_imageBounds)
	{
		if (IntRectangle::IsRectOverlapping(rect, IntRectangle(x, y, w, h)))
		{
			return false;
		}
	}
	return true;
}
