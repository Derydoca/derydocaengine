#include "EngineComponentsPch.h"
#include "Components\Light.h"

#include <iostream>
#include "Rendering\LightManager.h"
#include "Rendering\Material.h"
#include "Rendering\GraphicsAPI.h"
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Components
{

	Light::Light() :
		m_Type(LightType::Point),
		m_Color(1, 1, 1, 1),
		m_SpotlightExponent(0.0f),
		m_SpotlightCutoff(0.0f),
		m_MatrixStack(std::make_shared<Rendering::MatrixStack>())
	{
	}

	Light::~Light()
	{
	}

	glm::mat4 Light::getProjectionMatrix()
	{
		return glm::mat4();
	}

	glm::mat4 Light::getViewMatrix()
	{
		return glm::mat4();
	}

	void Light::init()
	{
		auto sp = std::static_pointer_cast<Light>(shared_from_this());
		Rendering::LightManager::getInstance().addLight(sp);
	}

	void Light::preDestroy()
	{
		auto sp = std::static_pointer_cast<Light>(shared_from_this());
		Rendering::LightManager::getInstance().removeLight(sp);
	}

	SERIALIZE_FUNC_LOAD(archive, Light)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Type),
			SERIALIZE(m_Color),
			SERIALIZE(m_SpotlightExponent),
			SERIALIZE(m_SpotlightCutoff)
		);
	}

	SERIALIZE_FUNC_SAVE(archive, Light)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Type),
			SERIALIZE(m_Color),
			SERIALIZE(m_SpotlightExponent),
			SERIALIZE(m_SpotlightCutoff)
		);
	}

}
