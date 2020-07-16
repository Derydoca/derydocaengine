#include "EngineComponentsPch.h"
#include "Terrain.h"

#include "Rendering\Material.h"
#include "Components\MeshRenderer.h"
#include "Rendering\ShaderLibrary.h"
#include "Rendering\Texture.h"

namespace DerydocaEngine::Components
{

	Terrain::Terrain() :
		m_Width(1),
		m_Depth(1),
		m_UnitScale(0.1f),
		m_HeightScale(0.1f),
		m_HeightData(nullptr),
		m_Mesh(),
		m_MeshRenderer()
	{
	}

	Terrain::Terrain(const std::string & fileName, float const& unitScale, float const& heightScale) :
		m_Width(1),
		m_Depth(1),
		m_UnitScale(unitScale),
		m_HeightScale(heightScale),
		m_HeightData(nullptr),
		m_Mesh(),
		m_MeshRenderer()
	{
		loadTerrainFromTexture(fileName, unitScale, heightScale);
	}

	Terrain::Terrain(int const& width, int const& depth, float const& unitScale, float const& heightScale) :
		m_Width(width),
		m_Depth(depth),
		m_UnitScale(unitScale),
		m_HeightScale(heightScale),
		m_HeightData(nullptr),
		m_Mesh(),
		m_MeshRenderer()
	{
		// Initialize the height map
		float tHeight = 0.0f;
		float incrementer = 0.05f;
		m_HeightData = new float*[depth];
		for (int i = 0; i < depth; ++i) {
			m_HeightData[i] = new float[width];
			for (int j = 0; j < width; j++)
			{
				m_HeightData[i][j] = ((i % 16) * 0.05f) + ((j % 32) * 0.05f);
				tHeight += 0.05f;
			}
			tHeight = 0.0f;
			incrementer *= -1;
		}

		Terrain::updateMesh();

		auto shader = Rendering::ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\diffuseFrag");
		auto mat = std::make_shared<Rendering::Material>();
		mat->setShader(shader);

		//m_meshRenderer = std::make_shared<MeshRenderer>(m_mesh, mat);
	}

	Terrain::~Terrain()
	{
		delete m_HeightData;
	}

	void Terrain::draw()
	{
		m_Mesh->draw();
	}

	void Terrain::updateMesh()
	{

		// Build the list of verts
		int numVerts = m_Width * m_Depth;
		std::vector<glm::vec3> positions(numVerts);
		std::vector<glm::vec3> normals(numVerts);
		std::vector<glm::vec2> texCoords(numVerts);
		for (int z = 0; z < m_Depth; z++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				int index = (z * m_Width) + x;

				// Calculate the normal
				float sx = m_HeightData[x < m_Width - 1 ? x + 1 : x][z] - m_HeightData[x > 0 ? x - 1 : x][z];
				if (x == 0 || x == m_Width - 1) {
					sx *= 2;
				}
				float sy = m_HeightData[x][z < m_Depth - 1 ? z + 1 : z] - m_HeightData[x][z > 0 ? z - 1 : z];
				if (z == 0 || z == m_Depth - 1) {
					sy *= 2;
				}
				glm::vec3 normal(-sx * m_HeightScale, 2 * m_UnitScale, -sy * m_HeightScale);
				normal = glm::normalize(normal);

				positions[index] = glm::vec3(x, m_HeightData[x][z] * m_HeightScale, z) * m_UnitScale;
				texCoords[index] = glm::vec2(x, z) * m_UnitScale;
				normals[index] = normal;
			}
		}

		// Build the list of triangles
		unsigned int numIndices = (((m_Width - 1) * (m_Depth - 1)) * 2) * 6;
		std::vector<unsigned int> indices(numIndices);
		for (int z = 0; z < m_Depth - 1; z++)
		{
			for (int x = 0; x < m_Width - 1; x++)
			{
				int q = (z * m_Width) + x;
				indices[(q * 6) + 0] = q + 0;
				indices[(q * 6) + 1] = q + m_Width + 0;
				indices[(q * 6) + 2] = q + 1;
				indices[(q * 6) + 3] = q + 1;
				indices[(q * 6) + 4] = q + m_Width + 0;
				indices[(q * 6) + 5] = q + m_Width + 1;
			}
		}

		m_Mesh = std::make_shared<Rendering::Mesh>(positions, indices, normals, texCoords);
	}

	void Terrain::setTextureSlot(int const& slot, std::shared_ptr<Rendering::Texture> texture)
	{
		m_MeshRenderer->getMaterial()->setTextureSlot(slot, texture);
	}

	void Terrain::init()
	{
		m_MeshRenderer->setGameObject(getGameObject());
	}

	void Terrain::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		m_MeshRenderer->render(matrixStack);
	}

	void Terrain::loadTerrainFromTexture(const std::string & fileName, float const& unitScale, float const& heightScale)
	{
		m_UnitScale = unitScale;
		m_HeightScale = heightScale;

		// Load the image data
		int w, h, bpp;
		unsigned char* data = stbi_load(fileName.c_str(), &w, &h, &bpp, 0);

		// Initialize the height map
		float tHeight = 0.0f;
		float incrementer = 0.05f;
		m_HeightData = new float*[m_Depth];
		for (int i = 0; i < m_Depth; ++i) {
			m_HeightData[i] = new float[m_Width];
			for (int j = 0; j < m_Width; j++)
			{
				m_HeightData[i][j] = data[(i * m_Width) + j] / 255.0f;
				tHeight += 0.05f;
			}
			tHeight = 0.0f;
			incrementer *= -1;
		}

		Terrain::updateMesh();

		auto shader = Rendering::ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\diffuseFrag");
		auto mat = std::make_shared<Rendering::Material>();
		mat->setShader(shader);

		//m_meshRenderer = std::make_shared<MeshRenderer>(m_mesh, mat);
	}

	SERIALIZE_FUNC_LOAD(archive, Terrain)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent));
	}

	SERIALIZE_FUNC_SAVE(archive, Terrain)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent));
	}

}
