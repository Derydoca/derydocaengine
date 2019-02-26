#include "EditorComponentsPch.h"
#include "AnimationViewerWindow.h"
#include "Editor\EditorRenderer.h"
#include "Dgui\ResourcePicker.h"
#include "Rendering\Mesh.h"

DerydocaEngine::Components::AnimationViewerWindow::AnimationViewerWindow() :
	SceneViewerWindow(),
	m_animationTime(0.0f),
	m_playing(false),
	m_meshRenderer(std::make_shared<Components::SkinnedMeshRenderer>()),
	m_scene(std::make_shared<Scenes::HardCodedScene>())
{
	auto meshRendererGameObject = std::make_shared<GameObject>("__skinnedMeshRenderer");
	meshRendererGameObject->addComponent(m_meshRenderer);
	meshRendererGameObject->init();
	meshRendererGameObject->postInit();

	m_scene->getRoot()->addChild(meshRendererGameObject);
}

DerydocaEngine::Components::AnimationViewerWindow::~AnimationViewerWindow()
{
}

void DerydocaEngine::Components::AnimationViewerWindow::update(const float deltaTime)
{
	SceneViewerWindow::update(deltaTime);

	if (m_playing)
	{
		m_animationTime += deltaTime;
	}
}

void DerydocaEngine::Components::AnimationViewerWindow::renderWindow()
{
	SceneViewerWindow::updateDisplayProperties();

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Animation", m_meshRenderer->getAnimationResource(), Resources::AnimationResourceType, modifiedResource))
		{
			m_meshRenderer->setAnimationResource(std::static_pointer_cast<Resources::AnimationResource>(modifiedResource));
		}
	}

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Mesh", m_meshRenderer->getMeshResource(), Resources::MeshResourceType, modifiedResource))
		{
			m_meshRenderer->setMeshResource(std::static_pointer_cast<Resources::MeshResource>(modifiedResource));
		}
	}

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Material", m_meshRenderer->getMaterialResource(), Resources::MaterialResourceType, modifiedResource))
		{
			m_meshRenderer->setMaterialResource(std::static_pointer_cast<Resources::MaterialResource>(modifiedResource));
		}
	}

	SceneViewerWindow::renderViewToWindow();
}

glm::vec2 DerydocaEngine::Components::AnimationViewerWindow::getViewPadding()
{
	return glm::vec2(0.0f, 150.0f);
}

void DerydocaEngine::Components::AnimationViewerWindow::renderToActiveBuffer()
{
	Editor::EditorRenderer::GetInstance().renderEditorCameraToActiveBuffer(getCamera(), {m_scene}, getDisplayWidth(), getDisplayHeight());
}
