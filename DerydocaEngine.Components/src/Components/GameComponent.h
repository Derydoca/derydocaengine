#pragma once

#include <boost\uuid\uuid.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "Components\GameComponentFactory.h"
#include "GameObject.h"
#include "ObjectLibrary.h"
#include "Helpers\YamlTools.h"
#include "TypeNameLookup.h"

struct Resource;

namespace DerydocaEngine {
	namespace Components {
		struct Transform;
	}
	namespace Rendering {
		struct Projection;
		class Material;
		class MatrixStack;
	}
}

namespace DerydocaEngine::Components
{

	template <typename T>
	class SelfRegister
	{
	protected:
		static bool s_isRegistered;
	};

	template <typename T>
	bool SelfRegister<T>::s_isRegistered = GameComponentFactory::getInstance().registerGenerator(T::getClassName(), T::generateInstance) && TypeNameLookup::get().registerType<T>(T::getClassName());

#define GENINSTANCE(TYPE) \
	static std::shared_ptr<Components::GameComponent> generateInstance() { return std::static_pointer_cast<Components::GameComponent>(std::make_shared<TYPE>()); }\
	static std::string getClassName() { return #TYPE; }\
	void __forceRegistration() { s_isRegistered; };\
	virtual unsigned long getTypeId() const { return DerydocaEngine::getTypeId<TYPE>(); }\

	class GameComponent: public std::enable_shared_from_this<GameComponent>, public Object {
	public:
		GameComponent() :
			m_gameObject()
		{}
		virtual ~GameComponent() {}

		// Game Component Lifecycle
		// ------------------------
		// COMPONENT CREATION
		// 	 - deserialize
		// 	 - init
		// 	 - postInit
		// GAME LOOP
		// 	 - update
		// 	 - preRender
		// 	 - render
		// 	 - postRender
		// 	 - renderEditorGUI
		// COMPONENT DESTRUCTION
		// 	 - preDestroy
		virtual void deserialize(const YAML::Node& compNode) {}
		virtual void init() {}
		virtual void postInit() {}
		virtual void postRender() {}
		virtual void preDestroy() {}
		virtual void preRender() {}
		virtual void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack) {}
		virtual void renderEditorGUI() {}
		virtual void update(const float deltaTime) {}

		virtual unsigned long getTypeId() const = 0;

		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			const std::shared_ptr<Transform> projectionTransform) {}

		inline void setGameObject(const std::weak_ptr<GameObject> gameObject) { m_gameObject = gameObject; }
		inline void setId(const boost::uuids::uuid& id) { m_id = id; }
		inline std::shared_ptr<GameObject> getGameObject() { return m_gameObject.lock(); }

		void destroy(std::shared_ptr<GameObject> objectToDestroy);

		template<typename T>
		inline std::shared_ptr<T> getComponentInChildren()
		{
			// Get the game object that this component belongs to
			auto gameObject = getGameObject();
			if (gameObject == nullptr)
			{
				return nullptr;
			}
			return gameObject->getComponentInChildren<T>();
		}

		template<typename T>
		inline std::shared_ptr<T> findComponentOfType()
		{
			std::shared_ptr<GameObject> root = getGameObject();
			while (root->hasParent())
			{
				root = root->getParent();
			}

			return root->getComponentInChildren<T>();
		}

		template<typename T>
		inline std::shared_ptr<T> findComponentOfType(boost::uuids::uuid id)
		{
			std::shared_ptr<GameObject> root = getGameObject();
			while (root->hasParent())
			{
				root = root->getParent();
			}

			return root->getComponentInChildren<T>(id);
		}

	protected:
		inline std::shared_ptr<Resources::Resource> getResource(const YAML::Node& node, const std::string& resourceName)
		{
			YAML::Node resourceNode = node[resourceName];

			if (resourceNode == nullptr)
			{
				return nullptr;
			}

			boost::uuids::uuid id = resourceNode.as<boost::uuids::uuid>();
			auto resource = ObjectLibrary::getInstance().getResource(id);
			return resource;
		}

		template<typename T>
		inline std::shared_ptr<T> getResource(const YAML::Node& node, const std::string& resourceName)
		{
			auto r = getResource(node, resourceName);
			return std::static_pointer_cast<T>(r);
		}

		template<typename T>
		inline std::shared_ptr<T> getResourcePointer(const boost::uuids::uuid& resourceId)
		{
			return ObjectLibrary::getInstance().getResourceObjectPointer<T>(resourceId);
		}

		template<typename T>
		inline std::shared_ptr<T> getResourcePointer(const YAML::Node& node, const std::string& resourceName)
		{
			YAML::Node resourceIdNode = node[resourceName];
			if (resourceIdNode == nullptr || !resourceIdNode.IsScalar())
			{
				return nullptr;
			}

			boost::uuids::uuid resourceId = resourceIdNode.as<boost::uuids::uuid>();
			if (resourceId.is_nil())
			{
				D_LOG_ERROR("Invalid resource ID");
				return nullptr;
			}

			return ObjectLibrary::getInstance().getResourceObjectPointer<T>(resourceId);
		}

	private:
		std::weak_ptr<GameObject> m_gameObject;
	};

}
