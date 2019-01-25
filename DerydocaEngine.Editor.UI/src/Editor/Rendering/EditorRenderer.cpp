#include "EditorPch.h"
#include "EditorRenderer.h"
#include "Settings\EngineSettings.h"
#include "Rendering\CameraManager.h"
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Editor::Rendering
{

	EditorRenderer::EditorRenderer() :
		RendererImplementation("Derydoca Engine - Editor", 300, 300),
		m_editorComponentsScene(std::make_shared<Scenes::SerializedScene>()),
		m_editorWindowsScene(std::make_shared<Scenes::SerializedScene>())
	{
	}

	EditorRenderer::~EditorRenderer()
	{
	}

	void EditorRenderer::init()
	{
		// Load the settings
		auto settings = DerydocaEngine::Settings::EngineSettings(".\\engineSettings.yaml");
		m_display.setSize(settings.getWidth(), settings.getHeight());
		m_display.init();

		// Load the scenes
		loadScene(settings.getEditorComponentsSceneIdentifier(), m_editorComponentsScene);
		loadScene(settings.getEditorWindowsSceneIdentifier(), m_editorWindowsScene);
	}

	void EditorRenderer::renderFrame(const float deltaTime)
	{
		auto scene = Scenes::SceneManager::getInstance().getActiveScene();

		// Update
		m_editorComponentsScene->getRoot()->update(deltaTime);
		m_editorWindowsScene->getRoot()->update(deltaTime);
		scene->getRoot()->update(deltaTime);

		// ImGui new frame

		// Render
		DerydocaEngine::Rendering::CameraManager::getInstance().render({
			m_editorComponentsScene,
			m_editorWindowsScene,
			scene
			});

		// Post render
		m_editorComponentsScene->getRoot()->postRender();
		m_editorWindowsScene->getRoot()->postRender();
		scene->getRoot()->postRender();

		// Render editor GUI
		//m_editorWindowsScene->getRoot()->renderEditorGUI();

	}

	std::shared_ptr<Resources::LevelResource> EditorRenderer::getSceneResource(const std::string& sceneId, const std::string& sceneType)
	{
		// Validate that the sceneId parameter is populated
		if (sceneId.empty())
		{
			std::cout << "No identifier was provided for the " << sceneType << " scene!\n";
			return nullptr;
		}

		// Get the scene resource object with the ID of sceneId
		auto resource = ObjectLibrary::getInstance().getResource<Resources::LevelResource>(sceneId);
		if (resource == nullptr)
		{
			std::cout << "Unable to find the " << sceneType << " scene with ID of '" << sceneId << "'!\n";
			return nullptr;
		}

		// Return the found resource
		return resource;
	}

	void EditorRenderer::loadScene(const std::string& sceneId, std::shared_ptr<Scenes::SerializedScene> scene)
	{
		auto editorComponentsSceneResource = getSceneResource(sceneId, "editor components");
		if (editorComponentsSceneResource != nullptr)
		{
			scene->LoadFromFile(editorComponentsSceneResource->getSourceFilePath());
			scene->setUp();
			scene->getRoot()->init();
			scene->getRoot()->postInit();
		}
	}

}
