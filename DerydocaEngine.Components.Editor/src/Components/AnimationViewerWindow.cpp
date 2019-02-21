#include "EditorComponentsPch.h"
#include "AnimationViewerWindow.h"
#include "Editor\EditorRenderer.h"

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
