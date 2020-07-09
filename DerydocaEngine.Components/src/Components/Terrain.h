#pragma once
#include "Components\GameComponent.h"
#include "Rendering\MatrixStack.h"
#include "Rendering\Mesh.h"

namespace DerydocaEngine {
	namespace Components {
		class MeshRenderer;
	}
	namespace Rendering {
		class Texture;
	}
}

namespace DerydocaEngine::Components
{

	class Terrain : public GameComponent, SelfRegister<Terrain>
	{
	public:
		GENINSTANCE(Terrain);
		SERIALIZE_FUNC_DEFINITIONS;
		Terrain();
		Terrain(const std::string& fileName, float const& unitScale, float const& heightScale);
		Terrain(int const& width, int const& depth, float const& unitScale, float const& heightScale);
		~Terrain();

		void draw();
		void updateMesh();

		void setTextureSlot(int const& slot, std::shared_ptr<Rendering::Texture> texture);
		void init();

		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack);
		void deserialize(const YAML::Node& node);

	private:
		int m_Width;
		int m_Depth;
		float m_UnitScale;
		float m_HeightScale;
		float** m_HeightData;
		std::shared_ptr<Rendering::Mesh> m_Mesh;
		std::shared_ptr<MeshRenderer> m_MeshRenderer;

		void loadTerrainFromTexture(const std::string & fileName, float const& unitScale, float const& heightScale);
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::Terrain, 0);
