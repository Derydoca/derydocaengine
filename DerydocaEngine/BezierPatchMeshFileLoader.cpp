#include "BezierPatchMeshFileLoader.h"

BezierPatchMeshFileLoader::BezierPatchMeshFileLoader()
{
}

BezierPatchMeshFileLoader::~BezierPatchMeshFileLoader()
{
}

BezierPatchMesh * BezierPatchMeshFileLoader::Load(const char * filePath)
{
	int numPatches;
	float* patchData = nullptr;

	ifstream file(filePath);
	string line;
	if (file.is_open())
	{
		// Get the number of patches from the first line in the file
		if (!getline(file, line))
		{
			cout << "Unable to read first line of file!" << endl;
		}
		numPatches = stoi(line, nullptr, 10);

		// Allocate data for the patch data based on the number of patches we found
		patchData = new float[numPatches * BezierPatchMesh::FLOATS_PER_PATCH];

		for (int patchIndex = 0; patchIndex < numPatches; patchIndex++)
		{
			if (!loadPatchData(file, patchIndex, patchData))
			{
				cout << "Unable to load file due to issue with patch " << patchIndex << "." << endl;
			}
		}
	}
	else
	{
		cout << "Unable to open file: " << filePath << endl;
	}

	BezierPatchMesh* bpm = new BezierPatchMesh();
	bpm->loadPatchData(numPatches, patchData);
	delete(patchData);
	return bpm;
}

bool BezierPatchMeshFileLoader::loadPatchData(istream& fileStream, int patch, float * patchData)
{
	string line;
	
	// Skip the first line of patch information for now until I understand more of what those values do
	while (line != "3 3")
	{
		if (!getline(fileStream, line))
		{
			cout << "Unable to read first line of patch!" << endl;
			return false;
		}
	}
	
	int patchDataIndex = patch * BezierPatchMesh::FLOATS_PER_PATCH;

	for (int vectorIndex = 0; vectorIndex < BezierPatchMesh::VECTORS_PER_PATCH; vectorIndex++)
	{
		for (int vectorComponentIndex = 0; vectorComponentIndex < BezierPatchMesh::FLOATS_PER_VECTOR; vectorComponentIndex++)
		{
			// Read the component from the file
			string componentAsString;
			fileStream >> componentAsString;
	
			// Convert the component to a float
			float component = stof(componentAsString);
	
			// Load the component into the patch data
			//int patchDataIndex = patch * BezierPatchMesh::FLOATS_PER_PATCH + vectorIndex * BezierPatchMesh::FLOATS_PER_VECTOR + vectorComponentIndex;
			//cout << patchDataIndex << ": " << component << endl;
			patchData[patchDataIndex++] = component;
		}
	}
	
	return true;
}
