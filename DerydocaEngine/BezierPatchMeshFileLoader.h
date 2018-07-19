#pragma once
#include "BezierPatchMesh.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class BezierPatchMeshFileLoader
{
public:
	BezierPatchMeshFileLoader();
	~BezierPatchMeshFileLoader();

	BezierPatchMesh* Load(const char* filePath);
private:
	bool loadPatchData(istream& fileStream, int patch, float* patchData);
};

