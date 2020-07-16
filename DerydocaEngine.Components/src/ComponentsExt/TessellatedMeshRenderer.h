#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Material.h"
#include "Ext\BezierPatchMesh.h"
#include "Ext\BezierPatchMeshResource.h"
#include "Resources\MaterialResource.h"

namespace DerydocaEngine::Ext
{

	class TessellatedMeshRenderer : public Components::GameComponent, private Components::SelfRegister<TessellatedMeshRenderer>
	{
	public:
		GENINSTANCE(TessellatedMeshRenderer);
		SERIALIZE_FUNC_DEFINITIONS;

		TessellatedMeshRenderer();
		~TessellatedMeshRenderer();

		virtual void init();
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			const std::shared_ptr<Components::Transform> projectionTransform
		);
		virtual void update(const float deltaTime);

		void updateMaterial();

		std::shared_ptr<Rendering::Material> getMaterial() const { return m_Material.As<Rendering::Material>(); }
		bool& getUseDynamicTessellation() { return m_UseDynamicTessellation; }
		int& getTessellationLevel() { return m_TessellationLevel; }
		int& getMinDynamicTessLevel() { return m_MinDynamicTessLevel; }
		int& getMaxDynamicTessLevel() { return m_MaxDynamicTessLevel; }
		float& getMinDynamicTessDistance() { return m_MinDynamicTessDistance; }
		float& getMaxDynamicTessDistance() { return m_MaxDynamicTessDistance; }
	private:
		int m_TessellationLevel;
		bool m_UseDynamicTessellation;
		float m_MinDynamicTessDistance;
		float m_MaxDynamicTessDistance;
		int m_MinDynamicTessLevel;
		int m_MaxDynamicTessLevel;
		ResourceRef<Resources::BezierPatchMeshResource> m_Mesh;
		ResourceRef<Resources::MaterialResource> m_Material;

		unsigned int m_VAO;
		unsigned int m_VBO;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::TessellatedMeshRenderer, 0);
