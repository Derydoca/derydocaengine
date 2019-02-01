#include "EditorPch.h"
#include "EditorRenderer.h"
#include "Settings\EngineSettings.h"
#include "Rendering\CameraManager.h"
#include "Rendering\Gui\DearImgui.h"
#include "Scenes\SceneManager.h"
#include "Editor\EditorGUI.h"
#include "Rendering\CameraManager.h"
#include "Rendering\GraphicsAPI.h"
#include "ObjectLibrary.h"

namespace DerydocaEngine::Editor
{

	EditorRenderer::EditorRenderer() :
		RendererImplementation("Derydoca Engine - Editor", 300, 300),
		m_editorComponentsScene(std::make_shared<Scenes::SerializedScene>()),
		m_editorGuiScene(std::make_shared<Scenes::SerializedScene>()),
		m_editorSkyboxMaterial(std::make_shared<Rendering::Material>())
	{
		DerydocaEngine::Rendering::Gui::DearImgui::init(m_display);
	}

	EditorRenderer::~EditorRenderer()
	{
		DerydocaEngine::Rendering::Gui::DearImgui::shutdown();
	}

	void EditorRenderer::init()
	{
		// Load the settings
		auto settings = DerydocaEngine::Settings::EngineSettings(".\\engineSettings.yaml");
		m_display->setSize(settings.getWidth(), settings.getHeight());
		m_display->init();

		// Load the editor skybox material
		auto skyboxIdString = settings.getEditorSkyboxMaterialIdentifier();
		if (skyboxIdString.size() > 0)
		{
			auto skyboxId = boost::lexical_cast<boost::uuids::uuid>(skyboxIdString);
			m_editorSkyboxMaterial = ObjectLibrary::getInstance().getResourceObjectPointer<Rendering::Material>(skyboxId);
		}

		// Load the scenes
		loadScene(settings.getEditorComponentsSceneIdentifier(), m_editorComponentsScene);
		loadScene(settings.getEditorGuiSceneIdentifier(), m_editorGuiScene);
	}

	void EditorRenderer::renderEditorCameraToActiveBuffer(std::shared_ptr<Components::Camera> camera, int textureW, int textureH)
	{
		// Render the scene
		auto scene = Scenes::SceneManager::getInstance().getActiveScene();
		Rendering::CameraManager::getInstance().setCurrentCamera(camera);
		if (scene != nullptr)
		{
			camera->renderScenesToActiveBuffer({ scene, m_editorComponentsScene }, textureW, textureH);
		}
		else
		{
			camera->renderScenesToActiveBuffer({ m_editorComponentsScene }, textureW, textureH);
		}

		// Re-bind the display as the render target
		m_display->bindAsRenderTarget();
	}

	void EditorRenderer::renderFrame(const float deltaTime)
	{
		// Initialize a new immediate mode GUI frame
		DerydocaEngine::Rendering::Gui::DearImgui::newFrame(m_display);

		// Update
		m_editorGuiScene->getRoot()->update(deltaTime);

		// Render editor GUI
		EditorGUI::getInstance().render();
		m_editorGuiScene->getRoot()->renderEditorGUI();

		// Render
		render(glm::mat4(), m_editorGuiScene);

		// Render the immediate mode GUI frame to the framebuffer
		DerydocaEngine::Rendering::Gui::DearImgui::render(m_display);

		// Post render
		m_editorGuiScene->getRoot()->postRender();
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
