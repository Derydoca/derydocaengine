#pragma once

namespace DerydocaEngine::Ext
{

	class BezierPatchMesh : public Object
	{
	public:
		const static int FLOATS_PER_VECTOR = 3;
		const static int VECTORS_PER_PATCH = 16;
		const static int FLOATS_PER_PATCH = VECTORS_PER_PATCH * FLOATS_PER_VECTOR;

		BezierPatchMesh();
		~BezierPatchMesh();

		virtual unsigned long getTypeId() const { return 0; }

		int getNumPatches() const { return m_numPatches; }
		void loadPatchData(int const& numPatches, float* const& patchData);
		float* getPatchData() const { return m_patchData; }
	private:
		int m_numPatches;
		float* m_patchData;

		void clearPatchData();
	};

}
