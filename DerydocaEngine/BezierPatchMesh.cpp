#include "BezierPatchMesh.h"
#include <algorithm>

namespace DerydocaEngine::Ext
{

	BezierPatchMesh::BezierPatchMesh()
	{
	}

	BezierPatchMesh::~BezierPatchMesh()
	{
		delete[] m_patchData;
	}

	void BezierPatchMesh::loadPatchData(int const& numPatches, float * const& patchData)
	{
		// Clear out any old data
		clearPatchData();

		// Load in the new data
		m_numPatches = numPatches;
		int patchDataSize = numPatches * FLOATS_PER_PATCH;
		m_patchData = new float[patchDataSize];
		//memcpy(m_patchData, patchData, patchDataSize * sizeof(float));
		for (int i = 0; i < patchDataSize; i++)
		{
			m_patchData[i] = patchData[i];
		}
	}

	void BezierPatchMesh::clearPatchData()
	{
		m_numPatches = 0;
		delete(m_patchData);
		m_patchData = nullptr;
	}

}
