#pragma once

#include <string>

namespace DerydocaEngine::Rendering {
	struct TextureParameters;
}

namespace DerydocaEngine::Rendering
{
	enum TextureWrapMode
	{
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER,
		MIRRORED_REPEAT,
		REPEAT,
		MIRROR_CLAMP_TO_EDGE
	};

	struct TextureParameters
	{
	public:
		TextureWrapMode getWrapModeS() const { return m_wrapS; }
		TextureWrapMode getWrapModeT() const { return m_wrapT; }

		void setWrapModeS(const TextureWrapMode mode) { m_wrapS = mode; }
		void setWrapModeT(const TextureWrapMode mode) { m_wrapT = mode; }

		static unsigned int textureWrapModeToOpenGL(const TextureWrapMode mode);
	private:
		TextureWrapMode m_wrapS;
		TextureWrapMode m_wrapT;
	};

	class Texture
	{
	public:
		Texture();
		Texture(const std::string& fileName, const TextureParameters* params = nullptr);
		Texture(
			const std::string& xpos,
			const std::string& xneg,
			const std::string& ypos,
			const std::string& yneg,
			const std::string& zpos,
			const std::string& zneg
		);
		~Texture();

		void bind(const unsigned int unit) const;
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		unsigned int getTextureType() const { return m_textureType; }
		unsigned int getRendererId() const { return m_rendererId; }
		void updateBuffer(
			unsigned char * data,
			const int width,
			const int height,
			const int channels,
			const TextureParameters* params
		);

		unsigned int channelsToPixelFormat(const int numChannels) const;
	protected:
		unsigned int m_rendererId;
		int m_width;
		int m_height;
		unsigned int m_textureType;
	private:
		Texture(Texture const& other) {}
		void operator=(Texture const& other) {};

		void deleteTexture();
	};

}
