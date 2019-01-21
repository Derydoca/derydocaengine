#pragma once
#include "Object.h"
#include "InspectorRenderer.h"
#include "Components\SkinnedMeshRenderer.h"

namespace DerydocaEngine::Editor::Inspector
{
	class InspectorRendererFactory
	{
	public:
		static InspectorRendererFactory& getInstance() {
			static InspectorRendererFactory m_instance;
			return m_instance;
		}

		template<typename T>
		std::shared_ptr<InspectorRenderer> getInspectorRenderer()
		{
			unsigned long typeId = getTypeId<T>();
			unsigned long otherTypeId = getTypeId<Components::SkinnedMeshRenderer>();
			auto it = m_renderFunctions.find(typeId);
			if (it == m_renderFunctions.end())
			{
				return nullptr;
			}
			else
			{
				return (*it).second;
			}
		}

		void registerUndefinedRenderer(std::shared_ptr<InspectorRenderer> renderer)
		{
			m_renderFunctions[0] = renderer;
		}

		template<typename T>
		bool registerRenderer(std::shared_ptr<InspectorRenderer> renderer)
		{
			unsigned long typeId = getTypeId<T>();
			m_renderFunctions[typeId] = renderer;
			return true;
		}

		void renderDefaultInspector(std::shared_ptr<Object> object)
		{
			auto it = m_renderFunctions.find(0);
			if (it != m_renderFunctions.end())
			{
				(*it).second->render(object);
			}
		}

		void renderInspector(std::shared_ptr<Object> object)
		{
			unsigned long typeId = object->getTypeId();
			auto it = m_renderFunctions.find(typeId);
			if (it == m_renderFunctions.end())
			{
				renderDefaultInspector(object);
			}
			else
			{
				(*it).second->render(object);
			}
		}

	private:
		InspectorRendererFactory();
		~InspectorRendererFactory();
		InspectorRendererFactory(const InspectorRendererFactory&);

		std::unordered_map<unsigned long, std::shared_ptr<InspectorRenderer>> m_renderFunctions;
	};

}
