#pragma once
#include "Editor\SelectionManager.h"
#include "Components\SceneViewerWindow.h"
#include "Animation\AnimationData.h"
#include "Components\SkinnedMeshRenderer.h"
#include "Scenes\HardCodedScene.h"

namespace DerydocaEngine::Components
{
	class AnimationViewerWindow : public SceneViewerWindow, SelfRegister<AnimationViewerWindow>
	{
	public:
		GENINSTANCE(AnimationViewerWindow);

		AnimationViewerWindow();
		~AnimationViewerWindow();
		
		virtual void update(const float deltaTime) override;
		virtual void renderWindow();
		virtual std::string getWindowTitle() { return "Animation Viewer"; }
		virtual ImGuiWindowFlags getWindowFlags() { return ImGuiWindowFlags_None; }
		virtual glm::vec2 getViewPadding() override;
		virtual void renderToActiveBuffer() override;

		std::shared_ptr<Components::SkinnedMeshRenderer> getSkinnedMeshRenderer() { return m_meshRenderer; }

	private:
		float m_animationTime;
		bool m_playing;
		std::shared_ptr<Components::SkinnedMeshRenderer> m_meshRenderer;
		std::shared_ptr<Scenes::HardCodedScene> m_scene;

	};
}
