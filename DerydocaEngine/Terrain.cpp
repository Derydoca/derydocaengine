#include "Terrain.h"
#include "Shader.h"
#include "ShaderManager.h"

Terrain::Terrain(const std::string & fileName, float unitScale, float heightScale) :
	m_unitScale(unitScale),
	m_heightScale(heightScale)
{
	int channels;
	unsigned char* image = SOIL_load_image(fileName.c_str(), &m_width, &m_depth, &channels, SOIL_LOAD_L);

	// Initialize the height map
	float tHeight = 0.0f;
	float incrementer = 0.05f;
	m_heightData = new float*[m_depth];
	for (int i = 0; i < m_depth; ++i) {
		m_heightData[i] = new float[m_width];
		for (int j = 0; j < m_width; j++)
		{
			m_heightData[i][j] = image[(i * m_width) + j] / 255.0f;
			tHeight += 0.05f;
		}
		tHeight = 0.0f;
		incrementer *= -1;
	}

	Terrain::updateMesh();

	Shader* shader = new Shader("../res/basicShader");
	Material* mat = new Material();
	mat->setShader(shader);

	m_meshRenderer = new MeshRenderer(m_mesh, mat);
}

Terrain::Terrain(int width, int depth, float unitScale, float heightScale) :
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

	Shader* shader = new Shader("../res/basicShader");
	Material* mat = new Material();
	mat->setShader(shader);

	m_meshRenderer = new MeshRenderer(m_mesh, mat);
}

Terrain::~Terrain()
{
	delete(m_mesh);
}

void Terrain::draw()
{
	m_mesh->draw();
}

void Terrain::updateMesh()
{

	// Build the list of verts
	int numVerts = m_width * m_depth;
	Vertex* verts = new Vertex[numVerts];
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

			verts[index] = Vertex(glm::vec3(x, m_heightData[x][z] * m_heightScale, z) * m_unitScale, glm::vec2(x, z) * m_unitScale, normal);
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

	m_mesh = new Mesh(verts, numVerts, indices, numIndices);
}

void Terrain::setTextureSlot(int slot, Texture * texture)
{
	m_meshRenderer->getMaterial()->setTextureSlot(slot, texture);
}

void Terrain::init()
{
	m_meshRenderer->setGameObject(getGameObject());
}

void Terrain::render(MatrixStack * matrixStack)
{
	m_meshRenderer->render(matrixStack);
}
