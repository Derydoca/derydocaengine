#include "Terrain.h"

#include "Material.h"
#include "ShaderLibrary.h"
#include "stb_image.h"

namespace DerydocaEngine::Components
{

	Terrain::Terrain()
	{
		m_unitScale = 0.1f;
		m_heightScale = 0.1f;
	}

	Terrain::Terrain(const std::string & fileName, float const& unitScale, float const& heightScale)
	{
		loadTerrainFromTexture(fileName, unitScale, heightScale);
	}

	Terrain::Terrain(int const& width, int const& depth, float const& unitScale, float const& heightScale) :
		m_width(width),
		m_depth(depth),
		m_unitScale(unitScale),
		m_heightScale(heightScale)
	{
		// Initialize the height map
		float tHeight = 0.0f;
		float incrementer = 0.05f;
		m_heightData = new float*[depth];
		for (int i = 0; i < depth; ++i) {
			m_heightData[i] = new float[width];
			for (int j = 0; j < width; j++)
			{
				m_heightData[i][j] = ((i % 16) * 0.05f) + ((j % 32) * 0.05f);
				tHeight += 0.05f;
			}
			tHeight = 0.0f;
			incrementer *= -1;
		}

		Terrain::updateMesh();

		Rendering::Shader* shader = Rendering::ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\diffuseFrag");
		Rendering::Material* mat = new Rendering::Material();
		mat->setShader(shader);

		m_meshRenderer = new MeshRenderer(m_mesh, mat);
	}

	Terrain::~Terrain()
	{
		delete m_mesh;
		delete m_heightData;
		delete m_meshRenderer;
	}

	void Terrain::draw()
	{
		m_mesh->draw();
	}

	void Terrain::updateMesh()
	{

		// Build the list of verts
		int numVerts = m_width * m_depth;
		glm::vec3* positions = new glm::vec3[numVerts];
		glm::vec3* normals = new glm::vec3[numVerts];
		glm::vec2* texCoords = new glm::vec2[numVerts];
		for (int z = 0; z < m_depth; z++)
		{
			for (int x = 0; x < m_width; x++)
			{
				int index = (z * m_width) + x;

				// Calculate the normal
				float sx = m_heightData[x < m_width - 1 ? x + 1 : x][z] - m_heightData[x > 0 ? x - 1 : x][z];
				if (x == 0 || x == m_width - 1) {
					sx *= 2;
				}
				float sy = m_heightData[x][z < m_depth - 1 ? z + 1 : z] - m_heightData[x][z > 0 ? z - 1 : z];
				if (z == 0 || z == m_depth - 1) {
					sy *= 2;
				}
				glm::vec3 normal(-sx * m_heightScale, 2 * m_unitScale, -sy * m_heightScale);
				normal = glm::normalize(normal);

				positions[index] = glm::vec3(x, m_heightData[x][z] * m_heightScale, z) * m_unitScale;
				texCoords[index] = glm::vec2(x, z) * m_unitScale;
				normals[index] = normal;
			}
		}

		// Build the list of triangles
		unsigned int numIndices = (((m_width - 1) * (m_depth - 1)) * 2) * 6;
		unsigned int* indices = new unsigned int[numIndices];
		for (int z = 0; z < m_depth - 1; z++)
		{
			for (int x = 0; x < m_width - 1; x++)
			{
				int q = (z * m_width) + x;
				indices[(q * 6) + 0] = q + 0;
				indices[(q * 6) + 1] = q + m_width + 0;
				indices[(q * 6) + 2] = q + 1;
				indices[(q * 6) + 3] = q + 1;
				indices[(q * 6) + 4] = q + m_width + 0;
				indices[(q * 6) + 5] = q + m_width + 1;
			}
		}

		m_mesh = new Rendering::Mesh();
		m_mesh->loadMeshComponentDataDEPRECATED(numVerts, positions, normals, texCoords, indices, numIndices);
	}

	void Terrain::setTextureSlot(int const& slot, Rendering::Texture * const& texture)
	{
		m_meshRenderer->getMaterial()->setTextureSlot(slot, texture);
	}

	void Terrain::init()
	{
		m_meshRenderer->setGameObject(getGameObject());
	}

	void Terrain::render(Rendering::MatrixStack * const& matrixStack)
	{
		m_meshRenderer->render(matrixStack);
	}

	void Terrain::deserialize(YAML::Node const& node)
	{
		YAML::Node heightmapNode = node["heightmap"];
		if (heightmapNode)
		{
			boost::uuids::uuid heightmapResourceId = heightmapNode.as<boost::uuids::uuid>();
			Resources::Resource* heightmapResource = ObjectLibrary::getInstance().getResource(heightmapResourceId);

			float unitScale = node["unitScale"].as<float>();
			float heightScale = node["heightScale"].as<float>();
			loadTerrainFromTexture(heightmapResource->getSourceFilePath(), unitScale, heightScale);
			if (node["material"])
			{
				Rendering::Material* material = getResourceObject<Rendering::Material>(node, "material");
				m_meshRenderer->setMaterial(material);
			}
		}
	}

	void Terrain::loadTerrainFromTexture(const std::string & fileName, float const& unitScale, float const& heightScale)
	{
		m_unitScale = unitScale;
		m_heightScale = heightScale;

		// Load the image data
		int w, h, bpp;
		unsigned char* data = stbi_load(fileName.c_str(), &w, &h, &bpp, 0);

		// Initialize the height map
		float tHeight = 0.0f;
		float incrementer = 0.05f;
		m_heightData = new float*[m_depth];
		for (int i = 0; i < m_depth; ++i) {
			m_heightData[i] = new float[m_width];
			for (int j = 0; j < m_width; j++)
			{
				m_heightData[i][j] = data[(i * m_width) + j] / 255.0f;
				tHeight += 0.05f;
			}
			tHeight = 0.0f;
			incrementer *= -1;
		}

		Terrain::updateMesh();

		Rendering::Shader* shader = Rendering::ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\diffuseFrag");
		Rendering::Material* mat = new Rendering::Material();
		mat->setShader(shader);

		m_meshRenderer = new MeshRenderer(m_mesh, mat);
	}

}
