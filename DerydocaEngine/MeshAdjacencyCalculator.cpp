#include "MeshAdjacencyCalculator.h"

namespace DerydocaEngine::Ext
{

	MeshAdjacencyCalculator::MeshAdjacencyCalculator()
	{
		m_uniqueFaces = std::vector<MeshAdjacencyFace>();
		m_posMap = std::map<aiVector3D, unsigned int>();
		m_indexMap = std::map<MeshAdjacencyEdge, MeshAdjacencyNeighbors>();
	}

	MeshAdjacencyCalculator::~MeshAdjacencyCalculator()
	{
	}

	void MeshAdjacencyCalculator::buildAdjacencyList(aiMesh * const& mesh, std::vector<unsigned int> & indices)
	{
		indices = std::vector<unsigned int>(mesh->mNumFaces * 3 * 2);

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];

			MeshAdjacencyFace unique;

			for (unsigned int j = 0; j < 3; j++)
			{
				unsigned int index = face.mIndices[j];
				aiVector3D& v = mesh->mVertices[index];

				if (m_posMap.find(v) == m_posMap.end())
				{
					m_posMap[v] = index;
				}
				else
				{
					index = m_posMap[v];
				}

				unique.setIndex(j, index);
			}

			m_uniqueFaces.push_back(unique);

			MeshAdjacencyEdge e1(unique.getIndex(0), unique.getIndex(1));
			MeshAdjacencyEdge e2(unique.getIndex(1), unique.getIndex(2));
			MeshAdjacencyEdge e3(unique.getIndex(2), unique.getIndex(0));

			m_indexMap[e1].addNeighbor(i);
			m_indexMap[e2].addNeighbor(i);
			m_indexMap[e3].addNeighbor(i);
		}

		int indexCounter = 0;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const MeshAdjacencyFace& face = m_uniqueFaces[i];

			for (unsigned int j = 0; j < 3; j++)
			{
				MeshAdjacencyEdge e(face.getIndex(j), face.getIndex((j + 1) % 3));
				MeshAdjacencyNeighbors n = m_indexMap[e];
				unsigned int otherTri = n.getOther(i);

				const MeshAdjacencyFace& otherFace = m_uniqueFaces[otherTri];
				unsigned int oppositeIndex = otherFace.getOppositeIndex(e);

				indices[indexCounter++] = (face.getIndex(j));
				indices[indexCounter++] = (oppositeIndex);
			}
		}
	}

}
