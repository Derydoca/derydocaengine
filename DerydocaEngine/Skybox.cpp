#include "Skybox.h"
#include "Mesh.h"

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
		glm::vec3* positions = new glm::vec3[8]();
		positions[0] = glm::vec3(-size, size, size);
		positions[1] = glm::vec3(size, size, size);
		positions[2] = glm::vec3(size, size, -size);
		positions[3] = glm::vec3(-size, size, -size);
		positions[4] = glm::vec3(-size, -size, size);
		positions[5] = glm::vec3(size, -size, size);
		positions[6] = glm::vec3(size, -size, -size);
		positions[7] = glm::vec3(-size, -size, -size);

		unsigned int indices[36] = {
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

		m_mesh = new Mesh(8, 36, positions, indices, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
	}

}
