#pragma once
#include <vector>
#include <map>
#include "Rectangle.h"
#include "TexturePackerImage.h"
#include "TexturePackerTextureData.h"
#include "IntRect.h"

namespace DerydocaEngine::Rendering {
	class Texture;
}

namespace DerydocaEngine::Utilities
{

	class TexturePacker
	{
	public:
		TexturePacker(int const& channels);
		~TexturePacker();

		void addImage(
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
			int const& channels);
		void packImages();
		bool getIsDirty() const { return m_isDirty; }
		Rendering::Texture* allocTexture() { return m_packedImageData.allocTexture(); };
		unsigned char* allocImageBuffer() { return m_packedImageData.allocImageBuffer(); }
		void freeSubImageData();
		std::vector<TexturePackerImage> getSubImageData() { return m_images; }
		int getWidth() const { return m_packedImageData.getWidth(); }
		int getHeight() const { return m_packedImageData.getHeight(); }
		int getChannels() const { return m_packedImageData.getChannels(); }
	private:
		std::vector<TexturePackerImage> m_images;
		std::map<unsigned long, unsigned char*> m_imageBuffers;
		bool m_isDirty;
		TexturePackerTextureData m_packedImageData;

		int getIntersectingImageXAdvance(std::vector<IntRect> imageBounds, IntRect rect);
	};

}
