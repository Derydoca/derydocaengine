#include "EditorPch.h"
#include "EditorRenderer.h"
#include "Settings\EngineSettings.h"
#include "Rendering\CameraManager.h"
#include "Rendering\Gui\DearImgui.h"
#include "Scenes\SceneManager.h"
#include "Editor\EditorGUI.h"
#include "Rendering\CameraManager.h"
#include "Rendering\GraphicsAPI.h"
#include "Rendering\LightManager.h"
#include "ObjectLibrary.h"

namespace DerydocaEngine::Editor
{
	const char* SETTINGS_PATH = "./EditorWindowSettings.json";

	EditorRenderer::EditorRenderer() :
		RendererImplementation("Derydoca Engine - Editor", 300, 300),
		m_editorComponentsScene(std::make_shared<Scenes::SerializedScene>()),
		m_editorGuiScene(std::make_shared<Scenes::SerializedScene>()),
		m_editorSkyboxMaterial(std::make_shared<Rendering::Material>()),
		m_settings()
	{
		DerydocaEngine::Rendering::Gui::DearImgui::init(m_display);
		try
		{
			std::ifstream fs(SETTINGS_PATH);
			if(fs.is_open())
			{
				cereal::JSONInputArchive iarchive(fs);
				iarchive(SERIALIZE_NAMED("Settings", m_settings));
			}
		}
		catch(std::string err)
		{
			m_settings = Settings::EditorWindowSettings();
		}
	}

	EditorRenderer::~EditorRenderer()
	{
		{
			m_settings.setPosition(m_display->getNonMaximizedPosition());
			m_settings.setSize(m_display->getNonMaximizedSize());
			m_settings.setState(m_display->getWindowState());
			
			std::ofstream fs(SETTINGS_PATH);
			cereal::JSONOutputArchive oarchive(fs);
			oarchive(SERIALIZE_NAMED("Settings", m_settings));
		}

		DerydocaEngine::Rendering::Gui::DearImgui::shutdown();
	}

	void EditorRenderer::init()
	{
		// Load the settings
		m_display->setSize(m_settings.getSize());
		int2 windowPosition = m_settings.getPosition();
		if (windowPosition.x >= 0)
		{
			m_display->setPosition(windowPosition);
		}
		DerydocaEngine::Settings::WindowState state = m_settings.getState();
		switch (state)
		{
		case DerydocaEngine::Settings::WindowState::Normal:
			break;
		case DerydocaEngine::Settings::WindowState::Maximized:
			m_display->maximize();
			break;
		case DerydocaEngine::Settings::WindowState::FullScreen:
			m_display->setFullScreen(true);
			break;
		default:
			break;
		}
		
		m_display->init();

		// Load the editor skybox material
		auto engineSettings = DerydocaEngine::Settings::EngineSettings::getInstance();
		auto skyboxIdString = engineSettings->getEditorSkyboxMaterialIdentifier();
		if (skyboxIdString.size() > 0)
		{
			auto skyboxId = boost::lexical_cast<boost::uuids::uuid>(skyboxIdString);
			m_editorSkyboxMaterial = ObjectLibrary::getInstance().getResourceObjectPointer<Rendering::Material>(skyboxId);
		}

		// Load the scenes
		loadScene(engineSettings->getEditorComponentsSceneIdentifier(), m_editorComponentsScene);
		loadScene(engineSettings->getEditorGuiSceneIdentifier(), m_editorGuiScene);
	}

	void EditorRenderer::renderEditorCameraToActiveBuffer(std::shared_ptr<Components::Camera> camera, int textureW, int textureH)
	{
		// Build a vector of the scenes to render
		auto scenes = std::vector<std::shared_ptr<Scenes::Scene>>();
		scenes.reserve(2);
		auto scene = Scenes::SceneManager::getInstance().getActiveScene();
		if (scene != nullptr)
		{
			scenes.push_back(scene);
		}
		scenes.push_back(m_editorComponentsScene);

		// Render the scenes
		renderEditorCameraToActiveBuffer(camera, scenes, textureW, textureH);
	}

	void EditorRenderer::renderEditorCameraToActiveBuffer(std::shared_ptr<Components::Camera> camera, std::vector<std::shared_ptr<Scenes::Scene>> scenes, int textureW, int textureH)
	{
		// Render the scenes
		Rendering::CameraManager::getInstance().renderCamerasToAttachedRenderTextures(scenes);
		Rendering::LightManager::getInstance().renderShadowMaps(scenes, camera->getGameObject()->getTransform());
		Rendering::CameraManager::getInstance().setCurrentCamera(camera);
		camera->renderScenesToActiveBuffer(scenes, textureW, textureH);

		// Re-bind the display as the render target
		m_display->bindAsRenderTarget();
	}

	void EditorRenderer::addWindow(const std::shared_ptr<Components::GameComponent> component)
	{
		auto windowGameObject = std::make_shared<GameObject>("__newWindow");
		windowGameObject->addComponent(component);
		windowGameObject->init();
		windowGameObject->postInit();
		m_editorGuiScene->getRoot()->addChild(windowGameObject);
	}
	void EditorRenderer::renderFrame(const float deltaTime)
	{
		// Initialize a new immediate mode GUI frame
		DerydocaEngine::Rendering::Gui::DearImgui::newFrame(m_display);

		// Update
		m_editorGuiScene->getRoot()->update(deltaTime);
		if (m_playing)
		{
			auto scene = Scenes::SceneManager::getInstance().getActiveScene();
			if (scene != nullptr)
			{
				scene->getRoot()->update(deltaTime);
			}
		}

		// Render editor GUI
		EditorGUI::getInstance().render();
		m_editorGuiScene->getRoot()->renderEditorGUI();

		// Render
		render(glm::mat4(), m_editorGuiScene);

		// Render the immediate mode GUI frame to the framebuffer
		DerydocaEngine::Rendering::Gui::DearImgui::render(m_display);

		// Post render
		m_editorGuiScene->getRoot()->postRender();

		m_editorGuiScene->getRoot()->destroyFlaggedChildren();
		m_editorComponentsScene->getRoot()->destroyFlaggedChildren();
	}

	std::shared_ptr<Resources::LevelResource> EditorRenderer::getSceneResource(const std::string& sceneId, const std::string& sceneType)
	{
		// Validate that the sceneId parameter is populated
		if (sceneId.empty())
		{
			D_LOG_ERROR("No identifier was provided for the {} scene.", sceneType);
			return nullptr;
		}

		// Get the scene resource object with the ID of sceneId
		auto resource = ObjectLibrary::getInstance().getResource<Resources::LevelResource>(sceneId);
		if (resource == nullptr)
		{
			D_LOG_ERROR("Unable to find the {} scene with ID of ''.", sceneType, sceneId);
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
