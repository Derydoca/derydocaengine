#include "EnginePch.h"
#include "BezierPatchMeshFileLoader.h"
#include <iostream>

namespace DerydocaEngine::Ext
{

	std::shared_ptr<BezierPatchMesh> BezierPatchMeshFileLoader::Load(const char * filePath)
	{
		int numPatches;
		float* patchData = nullptr;

		std::ifstream file(filePath);
		std::string line;
		if (file.is_open())
		{
			// Get the number of patches from the first line in the file
			if (!getline(file, line))
			{
				D_LOG_ERROR("Unable to read first line of file!");
			}
			numPatches = stoi(line, nullptr, 10);

			// Allocate data for the patch data based on the number of patches we found
			patchData = new float[numPatches * BezierPatchMesh::FLOATS_PER_PATCH];

			for (int patchIndex = 0; patchIndex < numPatches; patchIndex++)
			{
				if (!loadPatchData(file, patchIndex, patchData))
				{
					D_LOG_ERROR("Unable to load file due to issue with patch {}.", patchIndex);
				}
			}
		}
		else
		{
			D_LOG_ERROR("Unable to open file: {}", filePath);
		}

		auto bpm = std::make_shared<BezierPatchMesh>();
		bpm->loadPatchData(numPatches, patchData);
		delete(patchData);
		return bpm;
	}

	bool BezierPatchMeshFileLoader::loadPatchData(std::istream & fileStream, int const& patch, float * const& patchData)
	{
		std::string line;

		// Skip the first line of patch information for now until I understand more of what those values do
		while (line != "3 3")
		{
			if (!getline(fileStream, line))
			{
				D_LOG_ERROR("Unable to read first line of patch!");
				return false;
			}
		}

		int patchDataIndex = patch * BezierPatchMesh::FLOATS_PER_PATCH;

		for (int vectorIndex = 0; vectorIndex < BezierPatchMesh::VECTORS_PER_PATCH; vectorIndex++)
		{
			for (int vectorComponentIndex = 0; vectorComponentIndex < BezierPatchMesh::FLOATS_PER_VECTOR; vectorComponentIndex++)
			{
				// Read the component from the file
				std::string componentAsString;
				fileStream >> componentAsString;

				// Convert the component to a float
				float component = stof(componentAsString);

				// Load the component into the patch data
				patchData[patchDataIndex++] = component;
			}
		}

		return true;
	}

}
