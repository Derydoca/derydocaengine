#include "Mesh.h"

#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

Mesh::Mesh() {

}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].getPos());
		model.texCoords.push_back(*vertices[i].getTexCoord());
		model.normals.push_back(*vertices[i].getNormal());
	}

	for (unsigned int i = 0; i < numIndices; i++)
	{
		model.indices.push_back(indices[i]);
	}

	InitMesh(model);

}

Mesh::Mesh(const std::string& fileName)
{
	auto aiModel = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	int meshIndex = 0;
	int uvIndex = 0;

	if (meshIndex >= aiModel->mNumMeshes)
	{
		return;
	}

	aiMesh* mesh = aiModel->mMeshes[meshIndex];

	m_numIndices = mesh->mNumFaces * 3;

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	// Initialize the vert positions
	if (mesh->HasPositions())
	{
		float * verts = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			verts[i * 3 + 0] = mesh->mVertices[i].x;
			verts[i * 3 + 1] = mesh->mVertices[i].y;
			verts[i * 3 + 2] = mesh->mVertices[i].z;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), verts, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the texture coordinates buffer
	if (mesh->HasTextureCoords(uvIndex))
	{
		float * texCoords = new float[mesh->mNumVertices * 2];
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			texCoords[i * 2 + 0] = mesh->mTextureCoords[uvIndex][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[uvIndex][i].y;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * 2 * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the normals buffer
	if (mesh->HasNormals())
	{
		float * normals = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			normals[i * 3 + 0] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the indices buffer
	if (mesh->HasFaces())
	{
		unsigned int *indices = new unsigned int[m_numIndices];
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::InitMesh(const IndexedModel & model)
{
	m_numIndices = model.indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.texCoords.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::draw()
{
	glBindVertexArray(m_vertexArrayObject);

	glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}
