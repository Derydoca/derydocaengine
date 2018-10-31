#pragma once
#include "GameComponent.h"

namespace DerydocaEngine {
	namespace Components {
		class MeshRenderer;
		class SkinnedMeshRenderer;
	}
	namespace Ext {
		class TessellatedMeshRenderer;
	}
}

namespace DerydocaEngine::Components
{

	/*
	This will monitor the material source code and recompile/reapply the shader to the object that it is attached to whenever it changes.
	*/
	class MaterialRefresher : public GameComponent, SelfRegister<MaterialRefresher>
	{
	public:
		GENINSTANCE(MaterialRefresher);
		MaterialRefresher() {}
		~MaterialRefresher() {}

		void init();
		void update(float const& deltaTime);
		void refreshMaterial();
	private:
		bool isShaderSourceUpdated();
		int m_rendererType = 0;
		MeshRenderer* m_meshRenderer;
		SkinnedMeshRenderer* m_skinnedMeshRenderer;
		Ext::TessellatedMeshRenderer* m_tessMeshRenderer;
		bool m_vertexShaderExists = false;
		bool m_fragmentShaderExists = false;
		bool m_geometryShaderExists = false;
		bool m_tessEvalShaderExists = false;
		bool m_tessControlShaderExists = false;
		std::string m_shaderLoadPath;
		std::string m_vertexShaderPath;
		std::string m_fragmentShaderPath;
		std::string m_geometryShaderPath;
		std::string m_tessEvalShaderPath;
		std::string m_tessControlShaderPath;
		std::time_t m_vertexShaderModifiedTime;
		std::time_t m_fragmentShaderModifiedTime;
		std::time_t m_geometryShaderModifiedTime;
		std::time_t m_tessEvalShaderModifiedTime;
		std::time_t m_tessControlShaderModifiedTime;
	};

}
