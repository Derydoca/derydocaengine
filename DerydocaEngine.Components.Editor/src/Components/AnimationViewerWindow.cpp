#include "EditorComponentsPch.h"
#include "AnimationViewerWindow.h"
#include "Editor\EditorRenderer.h"
#include "Dgui\ResourcePicker.h"
#include "Rendering\Mesh.h"
#include <limits>

DerydocaEngine::Components::AnimationViewerWindow::AnimationViewerWindow() :
	SceneViewerWindow(),
	m_animationTime(0.0f),
	m_playbackSpeed(1.0f),
	m_playing(false),
	m_looping(false),
	m_meshRenderer(std::make_shared<Components::SkinnedMeshRenderer>()),
	m_scene(std::make_shared<Scenes::HardCodedScene>())
{
	auto meshRendererGameObject = std::make_shared<GameObject>("__skinnedMeshRenderer");
	meshRendererGameObject->addComponent(m_meshRenderer);
	meshRendererGameObject->init();
	meshRendererGameObject->postInit();
	meshRendererGameObject->getTransform()->setScale(glm::vec3(0.001f));

	m_scene->getRoot()->addChild(meshRendererGameObject);
}

DerydocaEngine::Components::AnimationViewerWindow::~AnimationViewerWindow()
{
}

void DerydocaEngine::Components::AnimationViewerWindow::update(const float deltaTime)
{
	SceneViewerWindow::update(deltaTime);

	if (m_playing && m_meshRenderer->isFullyConfigured())
	{
		m_animationTime += deltaTime * m_playbackSpeed;

		float animDuration = m_meshRenderer->getAnimation()->getDuration();
		if (m_animationTime > animDuration)
		{
			if (m_looping)
			{
				m_animationTime = fmod(m_animationTime, animDuration) * animDuration;
			}
			else
			{
				// TODO: Fix this. It is currently invalid to end on the final frame as
				//  the engine will crash looking for the next frame to blend
				m_animationTime = animDuration - (animDuration / 1000.0f);
				m_playing = false;
			}
		}
	}

	m_meshRenderer->setAnimationTime(m_animationTime);
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

	ImGui::SameLine();

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Mesh", m_meshRenderer->getMeshResource(), Resources::MeshResourceType, modifiedResource))
		{
			m_meshRenderer->setMeshResource(std::static_pointer_cast<Resources::MeshResource>(modifiedResource));
		}
	}

	ImGui::SameLine();

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Material", m_meshRenderer->getMaterialResource(), Resources::MaterialResourceType, modifiedResource))
		{
			m_meshRenderer->setMaterialResource(std::static_pointer_cast<Resources::MaterialResource>(modifiedResource));
		}
	}

	SceneViewerWindow::renderViewToWindow();

	renderTimelineControl();
}

glm::vec2 DerydocaEngine::Components::AnimationViewerWindow::getViewPadding()
{
	return glm::vec2(0.0f, 150.0f);
}

void DerydocaEngine::Components::AnimationViewerWindow::renderToActiveBuffer()
{
	Editor::EditorRenderer::GetInstance().renderEditorCameraToActiveBuffer(getCamera(), {m_scene}, getDisplayWidth(), getDisplayHeight());
}

void DerydocaEngine::Components::AnimationViewerWindow::renderTimelineControl()
{
	float animationDuration = 0.0f;
	auto anim = m_meshRenderer->getAnimation();
	if (anim)
	{
		animationDuration = anim->getDuration();
	}

	ImGui::PushItemWidth(-1);
	ImGui::SliderFloat("Time", &m_animationTime, 0.0f, animationDuration, "%.2f sec");
	ImGui::PopItemWidth();

	//ImGui::SameLine();
	if (ImGui::Button(m_playing ? "Pause" : "Play"))
	{
		m_playing = !m_playing;
	}

	ImGui::Checkbox("Loop", &m_looping);
	ImGui::InputFloat("Playback Speed", &m_playbackSpeed);
}
