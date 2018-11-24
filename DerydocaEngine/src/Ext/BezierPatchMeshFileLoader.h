#pragma once
#include "BezierPatchMesh.h"
#include <fstream>
#include <string>

namespace DerydocaEngine::Ext
{

	class BezierPatchMeshFileLoader
	{
	public:
		BezierPatchMeshFileLoader() {}
		~BezierPatchMeshFileLoader() {}

		BezierPatchMesh* Load(const char* filePath);
	private:
		bool loadPatchData(std::istream & fileStream, int const& patch, float* const& patchData);
	};

}
