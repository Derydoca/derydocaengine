#include "EditorComponentsPch.h"
#include "AnimationViewerWindow.h"
#include "Editor\EditorRenderer.h"
#include "Dgui\ResourcePicker.h"
#include "Rendering\Mesh.h"
#include <limits>
#include "Components\Light.h"
#include "SceneCameraInput\OrbitalControlStrategy.h"
#include "Components\MeshRenderer.h"

DerydocaEngine::Components::AnimationViewerWindow::AnimationViewerWindow() :
	SceneViewerWindow(std::make_shared<SceneCameraInput::OrbitalControlStrategy>()),
	m_AnimationTime(0.0f),
	m_PlaybackSpeed(1.0f),
	m_ModelScale(0.001f),
	m_Playing(false),
	m_Looping(true),
	m_MeshRenderer(std::make_shared<Components::SkinnedMeshRenderer>()),
	m_Scene(std::make_shared<Scenes::HardCodedScene>())
{
	m_Scene->setUp();
	auto meshRendererGameObject = std::make_shared<GameObject>("__skinnedMeshRenderer");
	meshRendererGameObject->addComponent(m_MeshRenderer);
	meshRendererGameObject->init();
	meshRendererGameObject->postInit();
	meshRendererGameObject->getTransform()->setScale(glm::vec3(0.001f));

	m_Scene->getRoot()->addChild(meshRendererGameObject);

	auto go = std::make_shared<GameObject>("");
	go->addComponent(Components::Light::generateInstance());
	go->init();
	go->postInit();
	go->getTransform()->setPosition({ 0.0f, 0.5f, 1.0f });
	m_Scene->getRoot()->addChild(go);
	auto go2 = std::make_shared<GameObject>("");
	go2->addComponent(Components::Light::generateInstance());
	go2->init();
	go2->postInit();
	go2->getTransform()->setPosition({ 0.0f, 0.5f, -1.0f });
	m_Scene->getRoot()->addChild(go2);
	auto goGrid = std::make_shared<GameObject>("_xzPlane");
	auto gridMeshRenderer = std::make_shared<Components::MeshRenderer>();
	gridMeshRenderer->setMesh(ObjectLibrary::getInstance().getResource<Resources::MeshResource>("cdadf7f6-536c-4765-9d6d-08a15c8e54c5"));
	gridMeshRenderer->setMaterial(ObjectLibrary::getInstance().getResource<Resources::MaterialResource>("f31bfa36-a1fe-44fd-8cee-0ee83f3ce69a"));
	gridMeshRenderer->setTransparent(true);
	goGrid->addComponent(gridMeshRenderer);
	goGrid->init();
	goGrid->postInit();
	m_Scene->getRoot()->addChild(goGrid);
}

DerydocaEngine::Components::AnimationViewerWindow::~AnimationViewerWindow()
{
	m_Scene->tearDown();
}

void DerydocaEngine::Components::AnimationViewerWindow::update(const float deltaTime)
{
	SceneViewerWindow::update(deltaTime);

	if (m_Playing && m_MeshRenderer->isFullyConfigured())
	{
		m_AnimationTime += deltaTime * m_PlaybackSpeed;

		float animDuration = static_cast<float>(m_MeshRenderer->getAnimation()->getDuration());
		if (m_AnimationTime > animDuration)
		{
			if (m_Looping)
			{
				m_AnimationTime = fmod(m_AnimationTime, animDuration) * animDuration;
			}
			else
			{
				// TODO: Fix this. It is currently invalid to end on the final frame as
				//  the engine will crash looking for the next frame to blend
				m_AnimationTime = animDuration - (animDuration / 1000.0f);
				m_Playing = false;
			}
		}
		if (m_AnimationTime < 0.0f)
		{
			if (m_Looping)
			{
				m_AnimationTime = animDuration - (fmod(m_AnimationTime, animDuration) * animDuration * -1);
			}
			else
			{
				m_AnimationTime = 0.0f;
				m_Playing = false;
			}
		}
	}

	m_MeshRenderer->setAnimationTime(m_AnimationTime);
}

void DerydocaEngine::Components::AnimationViewerWindow::renderWindow()
{
	SceneViewerWindow::updateDisplayProperties();

	{
		std::shared_ptr<Resources::Resource> modifiedResource = m_MeshRenderer->getMeshResource();
		if (Dgui::ResourcePicker("Mesh", m_MeshRenderer->getMeshResource(), Resources::MeshResourceType, modifiedResource))
		{
			m_MeshRenderer->setMeshResource(std::static_pointer_cast<Resources::MeshResource>(modifiedResource));
		}
	}

	ImGui::SameLine();
	{
		std::shared_ptr<Resources::Resource> modifiedResource = m_MeshRenderer->getAnimationResource();
		if (Dgui::ResourcePicker("Animation", m_MeshRenderer->getAnimationResource(), Resources::AnimationResourceType, modifiedResource))
		{
			m_MeshRenderer->setAnimationResource(std::static_pointer_cast<Resources::AnimationResource>(modifiedResource));
		}
	}

	ImGui::SameLine();
	{
		std::shared_ptr<Resources::Resource> modifiedResource = m_MeshRenderer->getMaterialResource();
		if (Dgui::ResourcePicker("Material", m_MeshRenderer->getMaterialResource(), Resources::MaterialResourceType, modifiedResource))
		{
			m_MeshRenderer->setMaterialResource(std::static_pointer_cast<Resources::MaterialResource>(modifiedResource));
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(60.0f);
	if (ImGui::DragFloat("Scale", &m_ModelScale, 0.01f))
	{
		updateModelScale();
	}
	ImGui::PopItemWidth();

	SceneViewerWindow::renderViewToWindow();

	renderTimelineControl();
}

glm::vec2 DerydocaEngine::Components::AnimationViewerWindow::getViewPadding()
{
	return glm::vec2(0.0f, 120.0f);
}

void DerydocaEngine::Components::AnimationViewerWindow::renderToActiveBuffer()
{
	Editor::EditorRenderer::GetInstance().renderEditorCameraToActiveBuffer(
		getCamera(),
		{m_Scene},
		static_cast<int>(getDisplayWidth()),
		static_cast<int>(getDisplayHeight())
	);
}

void DerydocaEngine::Components::AnimationViewerWindow::setModelScale(float modelScale)
{
	m_ModelScale = modelScale;
	updateModelScale();
}

void DerydocaEngine::Components::AnimationViewerWindow::renderTimelineControl()
{
	float animationDuration = 0.0f;
	auto anim = m_MeshRenderer->getAnimation();
	if (anim)
	{
		animationDuration = static_cast<float>(anim->getDuration());
	}

	if (ImGui::Button(m_Playing ? "Pause" : "Play"))
	{
		m_Playing = !m_Playing;
	}

	ImGui::SameLine();
	ImGui::Dummy({ 10.0f, 0.0f });
	ImGui::SameLine();

	ImGui::PushItemWidth(-1);
	ImGui::SliderFloat("Time", &m_AnimationTime, 0.0f, animationDuration, "%.2f sec");
	ImGui::PopItemWidth();

	ImGui::Checkbox("Loop", &m_Looping);
	ImGui::SameLine();
	ImGui::Dummy({10.0f, 0.0f});
	ImGui::SameLine();
	ImGui::PushItemWidth(50.0f);
	ImGui::InputFloat("Speed", &m_PlaybackSpeed);
	ImGui::PopItemWidth();
}

void DerydocaEngine::Components::AnimationViewerWindow::updateModelScale()
{
	m_MeshRenderer->getGameObject()->getTransform()->setScale(glm::vec3(m_ModelScale));
}
