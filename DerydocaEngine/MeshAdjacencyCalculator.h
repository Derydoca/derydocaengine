#pragma once
#include <vector>
#include <map>
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

class MeshAdjacencyEdge
{
public:
	MeshAdjacencyEdge(unsigned int index1, unsigned int index2)
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

	bool operator < (const MeshAdjacencyEdge& o) const
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

	bool sharesIndex(unsigned int index) { return m_index1 == index || m_index2 == index; }
private:
	unsigned int m_index1;
	unsigned int m_index2;
};

class MeshAdjacencyFace
{
public:
	MeshAdjacencyFace() {}
	~MeshAdjacencyFace() {}

	unsigned int getIndex(int i) const { return m_indices[i]; }
	void setIndex(int i, unsigned int value) { m_indices[i] = value; }
	unsigned int getOppositeIndex(MeshAdjacencyEdge e) const
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

	unsigned int getOther(unsigned int faceIndex) { return m_neighbors[0] == faceIndex ? m_neighbors[1] : m_neighbors[0];}
	void addNeighbor(unsigned int faceIndex)
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

	void buildAdjacencyList(const aiMesh* mesh, unsigned int* indices);
private:
	std::vector<MeshAdjacencyFace> m_uniqueFaces;
	std::map<aiVector3D, unsigned int> m_posMap;
	std::map<MeshAdjacencyEdge, MeshAdjacencyNeighbors> m_indexMap;
};
