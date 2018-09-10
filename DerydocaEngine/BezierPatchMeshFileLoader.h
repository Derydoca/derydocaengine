#pragma once
#include "BezierPatchMesh.h"
#include <iostream>
#include <fstream>
#include <string>

class BezierPatchMeshFileLoader
{
public:
	BezierPatchMeshFileLoader();
	~BezierPatchMeshFileLoader();

	BezierPatchMesh* Load(const char* filePath);
private:
	bool loadPatchData(std::istream & fileStream, int const& patch, float* const& patchData);
};

