#pragma once
#include "EditorPch.h"
#include "Components\EditorWindowComponent.h"
#include "Rendering\Display.h"
#include "Rendering\Material.h"
#include "Rendering\Renderer.h"
#include "Rendering\RenderTexture.h"
#include "Resources\LevelResource.h"
#include "Resources\MaterialResource.h"
#include "Scenes\SerializedScene.h"
#include "Settings\EditorWindowSettings.h"

namespace DerydocaEngine::Editor
{

	class EditorRenderer : public DerydocaEngine::Rendering::RendererImplementation
	{
	public:
		static EditorRenderer& GetInstance()
		{
			static EditorRenderer instance;
			return instance;
		}

		virtual void init() override;
		virtual void renderFrame(const float deltaTime) override;

		void renderEditorCameraToActiveBuffer(std::shared_ptr<Components::Camera> camera, int textureW, int textureH);
		void renderEditorCameraToActiveBuffer(std::shared_ptr<Components::Camera> camera, std::vector<std::shared_ptr<Scenes::Scene>> scenes, int textureW, int textureH);
		std::shared_ptr<Rendering::Material> getEditorSkyboxMaterial()
		{
			return std::static_pointer_cast<Rendering::Material>(m_editorSkyboxMaterialResource->getResourceObjectPointer());
		};
		std::shared_ptr<Resources::MaterialResource> getEditorSkyboxMaterialResource() { return m_editorSkyboxMaterialResource; };
		void addWindow(const std::shared_ptr<Components::GameComponent> component);

		bool isPlaying() const { return m_playing; }
		void setPlaying(bool isPlaying) { m_playing = isPlaying; }

	private:
		EditorRenderer();
		~EditorRenderer();
		EditorRenderer(const EditorRenderer&);

		std::shared_ptr<Resources::LevelResource> getSceneResource(const std::string& sceneId, const std::string& sceneType);
		void loadScene(const std::string& sceneId, std::shared_ptr<Scenes::SerializedScene> scene);

	private:
		bool m_playing;
		std::shared_ptr<Scenes::SerializedScene> m_editorComponentsScene;
		std::shared_ptr<Scenes::SerializedScene> m_editorGuiScene;
		std::shared_ptr<Resources::MaterialResource> m_editorSkyboxMaterialResource;
		Settings::EditorWindowSettings m_settings;

	};

}
