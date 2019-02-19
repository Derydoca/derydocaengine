#include "EditorComponentsPch.h"
#include "AnimationViewerWindow.h"

DerydocaEngine::Components::AnimationViewerWindow::AnimationViewerWindow() :
	SceneViewerWindow(),
	m_animationTime(0.0f),
	m_playing(false)
{
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
}

glm::vec2 DerydocaEngine::Components::AnimationViewerWindow::getViewPadding()
{
	return glm::vec2();
}
