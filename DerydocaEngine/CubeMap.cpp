#include "CubeMap.h"

CubeMap::CubeMap(const std::string & xpos, const std::string & xneg, const std::string & ypos, const std::string & yneg, const std::string & zpos, const std::string & zneg)
{
	const std::string cubeFaces[6] = { xpos, xneg, ypos, yneg, zpos, zneg };

	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	unsigned char* imageData;
	GLint width, height, components;

	// Hack to get the info necessary to create tex storage 2d
	imageData = stbi_load(cubeFaces[0].c_str(), &width, &height, &components, 4);
	free(imageData);

	glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA, width, height);

	for (int i = 0; i < 6; i++)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
		imageData = stbi_load(cubeFaces[i].c_str(), &width, &height, &components, 4);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, components, width, height, 0, components, GL_UNSIGNED_BYTE, imageData);
		free(imageData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}


CubeMap::~CubeMap()
{
}

void CubeMap::update(const Camera & camera)
{
	glm::mat4 model = camera.getViewProjection();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

void CubeMap::bind(unsigned int unit) {
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}