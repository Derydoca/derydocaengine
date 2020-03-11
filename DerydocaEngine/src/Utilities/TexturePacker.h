#pragma once
#include <vector>
#include <map>
#include "Rectangle.h"
#include "Utilities\TexturePackerImage.h"
#include "Utilities\TexturePackerTextureData.h"
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
			const unsigned long id,
			const float2& bearing,
			const float2& advance,
			const unsigned char* imageBuffer,
			const int2& size,
			const int channels);
		void packImages();
		bool getIsDirty() const { return m_isDirty; }
		std::shared_ptr<Rendering::Texture> allocTexture() { return m_packedImageData.allocTexture(); };
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

		int getIntersectingImageXAdvance(const std::vector<IntRect>& imageBounds, const IntRect& rect);
	};

}
