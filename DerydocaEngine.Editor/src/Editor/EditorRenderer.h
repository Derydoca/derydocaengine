#pragma once
#include "EditorPch.h"
#include "Rendering\Display.h"
#include "Rendering\Renderer.h"
#include "Rendering\RenderTexture.h"
#include "Scenes\SerializedScene.h"
#include "Resources\LevelResource.h"

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

		virtual void init();
		virtual void renderFrame(const float deltaTime);
		void renderEditorCamera(std::shared_ptr<Rendering::RenderTexture> renderTexture);

	private:
		EditorRenderer();
		~EditorRenderer();
		EditorRenderer(const EditorRenderer&);

		std::shared_ptr<Resources::LevelResource> getSceneResource(const std::string& sceneId, const std::string& sceneType);
		void loadScene(const std::string& sceneId, std::shared_ptr<Scenes::SerializedScene> scene);

	private:
		std::shared_ptr<Scenes::SerializedScene> m_editorComponentsScene;
		std::shared_ptr<Scenes::SerializedScene> m_editorGuiScene;

	};

}
