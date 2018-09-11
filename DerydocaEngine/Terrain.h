#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "GameComponent.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "Texture.h"

namespace DerydocaEngine::Components
{

	class Terrain : public GameComponent
	{
	public:
		GENINSTANCE(Terrain);
		Terrain();
		Terrain(const std::string& fileName, float const& unitScale, float const& heightScale);
		Terrain(int const& width, int const& depth, float const& unitScale, float const& heightScale);
		~Terrain();

		void draw();
		void updateMesh();

		void setTextureSlot(int const& slot, Texture* const& texture);
		void init();

		void render(MatrixStack* const& matrixStack);
		void deserialize(YAML::Node const& node);
	private:
		int m_width;
		int m_depth;
		float m_unitScale;
		float m_heightScale;
		float** m_heightData;
		Mesh* m_mesh;
		DerydocaEngine::Components::MeshRenderer* m_meshRenderer;

		void loadTerrainFromTexture(const std::string & fileName, float const& unitScale, float const& heightScale);
	};

}
