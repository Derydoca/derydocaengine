#pragma once
#include "EditorPch.h"
#include "Rendering\Renderer.h"
#include "Rendering\Display.h"
#include "Scenes\SerializedScene.h"
#include "Resources\LevelResource.h"

namespace DerydocaEngine::Editor::Rendering
{

	class EditorRenderer : public DerydocaEngine::Rendering::RendererImplementation
	{
	public:
		EditorRenderer();
		~EditorRenderer();

		virtual void init();
		virtual void renderFrame(const float deltaTime);

	private:
		std::shared_ptr<Resources::LevelResource> getSceneResource(const std::string& sceneId, const std::string& sceneType);
		void loadScene(const std::string& sceneId, std::shared_ptr<Scenes::SerializedScene> scene);

	private:
		std::shared_ptr<Scenes::SerializedScene> m_editorComponentsScene;
		std::shared_ptr<Scenes::SerializedScene> m_editorWindowsScene;

	};

}
