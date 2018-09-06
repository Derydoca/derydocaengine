#pragma once
#include <vector>
#include <map>
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

class MeshAdjacencyEdge
{
public:
	MeshAdjacencyEdge(unsigned int const& index1, unsigned int const& index2)
	{
		if (index1 < index2)
		{
			m_index1 = index1;
			m_index2 = index2;
		}
		else
		{
			m_index1 = index2;
			m_index2 = index1;
		}
	}
	~MeshAdjacencyEdge() {}

	bool operator < (MeshAdjacencyEdge const& o) const
	{
		if (m_index1 != o.m_index1)
		{
			return m_index1 < o.m_index1;
		}
		else if (m_index2 != o.m_index2)
		{
			return m_index2 < o.m_index2;
		}
		else
		{
			return false;
		}
	}

	bool sharesIndex(unsigned int const& index) const { return m_index1 == index || m_index2 == index; }
private:
	unsigned int m_index1;
	unsigned int m_index2;
};

class MeshAdjacencyFace
{
public:
	MeshAdjacencyFace() {}
	~MeshAdjacencyFace() {}

	unsigned int getIndex(int const& i) const { return m_indices[i]; }
	void setIndex(int const& i, unsigned int const& value) { m_indices[i] = value; }
	unsigned int getOppositeIndex(MeshAdjacencyEdge const& e) const
	{
		for (int i = 0; i < 3; i++)
		{
			if (!e.sharesIndex(m_indices[i]))
			{
				return m_indices[i];
			}
		}
		return 0;
	};
private:
	unsigned int m_indices[3];
};

class MeshAdjacencyNeighbors
{
public:
	MeshAdjacencyNeighbors() {}
	~MeshAdjacencyNeighbors() {}

	unsigned int getOther(unsigned int const& faceIndex) { return m_neighbors[0] == faceIndex ? m_neighbors[1] : m_neighbors[0];}
	void addNeighbor(unsigned int const& faceIndex)
	{
		m_neighbors[m_currentNeighbor] = faceIndex;
		m_currentNeighbor = (m_currentNeighbor + 1) % 2;
	}
private:
	unsigned int m_neighbors[2] = { 0, 0 };
	int m_currentNeighbor = 0;
};

class MeshAdjacencyCalculator
{
public:
	MeshAdjacencyCalculator();
	~MeshAdjacencyCalculator();

	void buildAdjacencyList(aiMesh* const& mesh, unsigned int* const& indices);
private:
	std::vector<MeshAdjacencyFace> m_uniqueFaces;
	std::map<aiVector3D, unsigned int> m_posMap;
	std::map<MeshAdjacencyEdge, MeshAdjacencyNeighbors> m_indexMap;
};
