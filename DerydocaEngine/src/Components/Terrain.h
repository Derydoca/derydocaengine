#pragma once
#include "Components\GameComponent.h"
#include "MatrixStack.h"
#include "Mesh.h"

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
		Terrain();
		Terrain(const std::string& fileName, float const& unitScale, float const& heightScale);
		Terrain(int const& width, int const& depth, float const& unitScale, float const& heightScale);
		~Terrain();

		void draw();
		void updateMesh();

		void setTextureSlot(int const& slot, Rendering::Texture* const& texture);
		void init();

		void render(Rendering::MatrixStack* const& matrixStack);
		void deserialize(YAML::Node const& node);
	private:
		int m_width;
		int m_depth;
		float m_unitScale;
		float m_heightScale;
		float** m_heightData;
		std::shared_ptr<Rendering::Mesh> m_mesh;
		MeshRenderer* m_meshRenderer;

		void loadTerrainFromTexture(const std::string & fileName, float const& unitScale, float const& heightScale);
	};

}
