#pragma once
#include "BezierPatchMesh.h"
#include <fstream>
#include <memory>
#include <string>

namespace DerydocaEngine::Ext
{

	class BezierPatchMeshFileLoader
	{
	public:
		BezierPatchMeshFileLoader() {}
		~BezierPatchMeshFileLoader() {}

		std::shared_ptr<BezierPatchMesh> Load(const char* filePath);
	private:
		bool loadPatchData(std::istream & fileStream, int const& patch, float* const& patchData);
	};

}
