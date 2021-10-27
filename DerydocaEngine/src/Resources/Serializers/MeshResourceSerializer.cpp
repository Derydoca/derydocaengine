#include "EnginePch.h"
#include "Resources\Serializers\MeshResourceSerializer.h"
#include "Resources\MeshResource.h"
#include "Rendering\Mesh.h"
#include "assimp\importer.hpp"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include "MeshAdjacencyCalculator.h"

namespace DerydocaEngine::Resources::Serializers
{
	std::shared_ptr<void> MeshResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		auto mr = std::static_pointer_cast<MeshResource>(resource);

		Assimp::Importer importer;

		const aiScene* aiScene = importer.ReadFile(resource->getSourceFilePath().c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		int uvIndex = 0;

		// Find the mesh index
		unsigned int meshIndex = 0;
		for (unsigned int i = 0; i < aiScene->mNumMeshes; i++)
		{
			std::string meshName = aiScene->mMeshes[i]->mName.data;
			if (meshName == mr->getMeshName())
			{
				meshIndex = i;
				break;
			}
		}

		aiMesh* mesh = aiScene->mMeshes[meshIndex];

		std::vector<glm::vec3> m_positions;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_texCoords;
		std::vector<unsigned int> m_indices;
		std::vector<glm::vec3> m_tangents;
		std::vector<glm::vec3> m_bitangents;
		Rendering::MeshFlags m_flags = mr->getFlags();

		ProcessMeshData(mesh, m_positions, uvIndex, m_texCoords, m_normals, m_flags, m_indices, m_tangents, m_bitangents);

		std::shared_ptr<Rendering::Mesh> m = std::make_shared<Rendering::Mesh>(
			m_positions,
			m_indices,
			m_normals,
			m_texCoords,
			m_tangents,
			m_bitangents,
			std::vector<Color>());

		m->setFlags(mr->getFlags());

		return std::static_pointer_cast<void>(m);
	}

	void MeshResourceSerializer::ProcessMeshData(
		aiMesh * &mesh,
		std::vector<glm::vec3> &m_positions,
		int uvIndex,
		std::vector<glm::vec2> &m_texCoords,
		std::vector<glm::vec3> &m_normals,
		DerydocaEngine::Rendering::MeshFlags m_flags,
		std::vector<unsigned int> &m_indices,
		std::vector<glm::vec3> &m_tangents,
		std::vector<glm::vec3> &m_bitangents)
	{
		if (mesh->HasPositions())
		{
			m_positions.reserve(mesh->mNumVertices);
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				m_positions.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
			}
		}

		if (mesh->HasTextureCoords(uvIndex))
		{
			m_texCoords.reserve(mesh->mNumVertices);
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				m_texCoords.push_back(glm::vec2(mesh->mTextureCoords[uvIndex][i].x, mesh->mTextureCoords[uvIndex][i].y));
			}
		}

		if (mesh->HasNormals())
		{
			m_normals.reserve(mesh->mNumVertices);
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				m_normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
			}
		}

		if (mesh->HasFaces())
		{
			if (m_flags & Rendering::MeshFlags::load_adjacent)
			{
				Ext::MeshAdjacencyCalculator mac;
				mac.buildAdjacencyList(mesh, m_indices);
			}
			else
			{
				m_indices = std::vector<unsigned int>(mesh->mNumFaces * 3);
				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					m_indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
					m_indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
					m_indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
				}
			}
		}

		if (mesh->HasTangentsAndBitangents())
		{
			m_tangents.reserve(mesh->mNumVertices);
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				m_tangents.push_back(glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
			}

			m_bitangents.reserve(mesh->mNumVertices);
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				m_bitangents.push_back(glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z));
			}
		}
	}

}
