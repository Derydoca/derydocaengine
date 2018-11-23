#pragma once
#include <glm\glm.hpp>
#include "DebugLine.h"
#include "Shader.h"
#include "Components\GameComponent.h"

namespace DerydocaEngine::Debug {

	class DebugVisualizer : public Components::GameComponent, Components::SelfRegister<DebugVisualizer>
	{
	public:
		GENINSTANCE(DebugVisualizer);
		DebugVisualizer();
		~DebugVisualizer();

		void draw(glm::mat4 const& worldMatrix);

		void postRender();
	private:
		void clear();

		DebugLine* m_lines;
		int m_lineCount;

		DebugLine m_testLine;

		Rendering::Shader* m_shader;
	};

}