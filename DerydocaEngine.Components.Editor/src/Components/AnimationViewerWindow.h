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
		virtual ~AnimationViewerWindow();

		virtual void renderWindow();
		virtual void update(const float deltaTime) override;

		virtual glm::ivec2 getWindowInitialSize() override { return { 500.0f, 400.0f }; }
		virtual std::string getWindowTitle() override { return "Animation Viewer"; }
		virtual ImGuiWindowFlags getWindowFlags() override { return ImGuiWindowFlags_None; }
		virtual glm::vec2 getViewPadding() override;
		virtual void renderToActiveBuffer() override;

		std::shared_ptr<Components::SkinnedMeshRenderer> getSkinnedMeshRenderer() { return m_meshRenderer; }
		void setModelScale(float modelScale);
		float getModelScale() { return m_modelScale; }

	private:
		void renderTimelineControl();
		void updateModelScale();

	private:
		float m_animationTime;
		float m_playbackSpeed;
		float m_modelScale;
		bool m_playing;
		bool m_looping;
		std::shared_ptr<Components::SkinnedMeshRenderer> m_meshRenderer;
		std::shared_ptr<Scenes::HardCodedScene> m_scene;
	};
}
