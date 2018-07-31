#include "TextRenderer.h"
#include "TexturePackerImage.h"
#include "YamlTools.h"

const int CARRIAGE_RETURN_CHAR = 13;
const int DEL_CHAR = 127;

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::init()
{
	m_meshRenderer = getComponent<MeshRenderer>();
}

void TextRenderer::postInit()
{
	if (m_fontFace && m_meshRenderer)
	{
		Texture* fontTexture = m_fontFace->getTexture();
		m_meshRenderer->getMaterial()->setTexture("CharacterSheet", fontTexture);
		m_texSize = ivec2(fontTexture->getWidth(), fontTexture->getHeight());
	}

	updateText();
}

void TextRenderer::update(float deltaTime)
{
}

void TextRenderer::render(MatrixStack * matrixStack)
{
	m_meshRenderer->render(matrixStack);
}

void TextRenderer::deserialize(YAML::Node compNode)
{
	YAML::Node fontSizeNode = compNode["fontSize"];
	if (fontSizeNode)
	{
		m_fontSize = fontSizeNode.as<float>();
	}

	YAML::Node textNode = compNode["text"];
	if (textNode)
	{
		m_text = textNode.as<string>();
	}

	YAML::Node boundsNode = compNode["bounds"];
	if (boundsNode)
	{
		m_bounds = boundsNode.as<vec2>();
	}

	auto fontResource = getResource<Resource*>(compNode, "font");
	if (fontResource->getType() == ResourceType::FontResourceType) {
		m_fontFace = new FontFace();
		m_fontFace->setFontSize(m_fontSize);
		m_fontFace->loadFromFontFile(fontResource->getSourceFilePath());
	}
	else if (fontResource->getType() == ResourceType::RasterFontResourceType)
	{
		m_fontFace = new FontFace();
		m_fontFace->loadFromSerializedFile(fontResource->getSourceFilePath());
	}
}

void TextRenderer::updateText()
{
	if (m_texSize.x == 0 || m_texSize.y == 0)
	{
		return;
	}

	delete[] m_verts;
	delete[] m_uvs;
	delete[] m_indices;

	int charCount = 0;
	for (int i = 0; i < m_text.length(); i++)
	{
		// Skip all characters that do not output any quads
		if (m_text[i] <= 31 || m_text[i] == DEL_CHAR)
		{
			continue;
		}
		charCount++;
	}

	int vertCount = charCount * 4;
	int indicesCount = charCount * 6;

	m_verts = new vec3[vertCount];
	m_uvs = new vec2[vertCount];
	m_indices = new unsigned int[indicesCount];

	float xPos = 0.0f;
	float yPos = 0.0f;
	float charWidth = 0.1f;
	float pointScale = 0.01f;

	int charQuadIndex = 0;
	for (int i = 0; i < m_text.length(); i++)
	{
		// Handle carriage returns
		if (m_text[i] == CARRIAGE_RETURN_CHAR)
		{
			yPos -= 0.65f;
			xPos = 0.0f;
			continue;
		}

		// Skip all characters that do not output any quads
		if (m_text[i] <= 31 || m_text[i] == DEL_CHAR)
		{
			continue;
		}

		TexturePackerImage img = m_fontFace->getCharData(m_text[i]);
		IntRectangle rect = img.getTexSheetPosition();
		float minx = (float)rect.getX() / (float)m_texSize.x;
		float maxx = ((float)rect.getX() + (float)rect.getWidth()) / (float)m_texSize.x;
		float miny = (float)rect.getY() / (float)m_texSize.y;
		float maxy = ((float)rect.getY() + (float)rect.getHeight()) / (float)m_texSize.y;

		// Set the UV positions
		m_uvs[charQuadIndex * 4 + 1] = vec2(minx, maxy);
		m_uvs[charQuadIndex * 4 + 0] = vec2(minx, miny);
		m_uvs[charQuadIndex * 4 + 3] = vec2(maxx, miny);
		m_uvs[charQuadIndex * 4 + 2] = vec2(maxx, maxy);

		// Set the indices to draw the two triangles
		m_indices[charQuadIndex * 6 + 0] = charQuadIndex * 4 + 0;
		m_indices[charQuadIndex * 6 + 1] = charQuadIndex * 4 + 1;
		m_indices[charQuadIndex * 6 + 2] = charQuadIndex * 4 + 2;
		m_indices[charQuadIndex * 6 + 3] = charQuadIndex * 4 + 0;
		m_indices[charQuadIndex * 6 + 4] = charQuadIndex * 4 + 2;
		m_indices[charQuadIndex * 6 + 5] = charQuadIndex * 4 + 3;

		// Set the vertex positions
		float charYMin = yPos + ((img.getBearingY()) * pointScale);
		float charYMax = yPos - (((img.getHeight() - img.getBearingY())) * pointScale);
		float charXMin = xPos + ((img.getBearingX()) * pointScale);
		float charXMax = xPos + (((img.getBearingX() + img.getWidth())) * pointScale);
		m_verts[charQuadIndex * 4 + 0] = vec3(charXMin, charYMin, 0);
		m_verts[charQuadIndex * 4 + 1] = vec3(charXMin, charYMax, 0);
		m_verts[charQuadIndex * 4 + 2] = vec3(charXMax, charYMax, 0);
		m_verts[charQuadIndex * 4 + 3] = vec3(charXMax, charYMin, 0);

		xPos += ((img.getAdvanceX() / 64.0f) * pointScale);
		yPos += ((img.getAdvanceY() / 64.0f) * pointScale);

		charQuadIndex++;
	}

	m_mesh.load(vertCount, m_verts, nullptr, m_uvs, m_indices, indicesCount);
	m_meshRenderer->setMesh(&m_mesh);
}
