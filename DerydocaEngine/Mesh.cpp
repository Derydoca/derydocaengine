#include "Mesh.h"

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
	IndexedModel model = OBJModel(fileName).ToIndexedModel();
	InitMesh(model);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::InitMesh(const IndexedModel & model)
{
	m_drawCount = model.indices.size();

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

	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
