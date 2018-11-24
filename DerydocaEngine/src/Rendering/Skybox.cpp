#include "Rendering\Skybox.h"
#include "Rendering\Mesh.h"

namespace DerydocaEngine::Rendering
{

	Skybox::Skybox()
	{
		buildMesh(100.0f);
	}

	Skybox::Skybox(float const& size)
	{
		buildMesh(size);
	}

	Skybox::~Skybox()
	{
	}

	void Skybox::buildMesh(float const& size)
	{
		std::vector<glm::vec3> positions = {
			glm::vec3(-size, size, size),
			glm::vec3(size, size, size),
			glm::vec3(size, size, -size),
			glm::vec3(-size, size, -size),
			glm::vec3(-size, -size, size),
			glm::vec3(size, -size, size),
			glm::vec3(size, -size, -size),
			glm::vec3(-size, -size, -size)
		};

		std::vector<unsigned int> indices = {
			// Top
			2, 1, 0,
			3, 2, 0,

			// Right
			1, 2, 5,
			5, 2, 6,

			// Bottom
			4, 5, 6,
			4, 6, 7,

			// Left
			0, 4, 7,
			3, 0, 7,

			// Front
			0, 1, 4,
			1, 5, 4,

			// Back
			2, 3, 6,
			3, 7, 6
		};

		m_mesh = new Mesh(positions, indices);
	}

}
