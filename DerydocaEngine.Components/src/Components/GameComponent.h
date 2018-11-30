#pragma once

#include <boost\uuid\uuid.hpp>
#include <iostream>
#include <vector>
#include "Components\GameComponentFactory.h"
#include "GameObject.h"
#include "ObjectLibrary.h"
#include "Helpers\YamlTools.h"

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
class YAML::Node;

namespace DerydocaEngine::Components
{

	template <typename T>
	class SelfRegister
	{
	protected:
		static bool s_isRegistered;
	};

	template <typename T>
	bool SelfRegister<T>::s_isRegistered = DerydocaEngine::Components::GameComponentFactory::getInstance().registerGenerator(T::getClassName(), T::generateInstance);

#define GENINSTANCE(TYPE) \
	static GameComponent* generateInstance() { return new TYPE(); }\
	static std::string getClassName() { return #TYPE; }\
	void __forceRegistration() { s_isRegistered; };

	class GameComponent {
	public:
		GameComponent() {}
		virtual ~GameComponent() {}
		virtual void init() {}
		virtual void postInit() {}
		virtual void update(float const& deltaTime) {}
		virtual void preRender() {}
		virtual void render(std::shared_ptr<Rendering::MatrixStack> const matrixStack) {}
		virtual void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			const std::shared_ptr<Transform> projectionTransform) {}
		virtual void postRender() {}
		inline void setGameObject(const std::shared_ptr<GameObject> gameObject) { m_gameObject = gameObject; }
		inline std::shared_ptr<GameObject> getGameObject() { return m_gameObject; }
		virtual void deserialize(YAML::Node const& compNode) { };

		template<typename T>
		inline T* getComponent()
		{
			// Get the game object that this component belongs to
			std::shared_ptr<GameObject> gameObject = getGameObject();
			if (gameObject == nullptr)
			{
				return nullptr;
			}

			// Iterate through all of the components on this game object
			std::vector<GameComponent*> gameObjectComponents = getGameObject()->getComponents();
			for (std::vector<GameComponent*>::iterator it = gameObjectComponents.begin(); it != gameObjectComponents.end(); ++it)
			{
				// Attempt to cast this object to the type we want
				GameComponent* component = *it;
				T* attemptedCast = dynamic_cast<T*>(component);

				// If the cast worked, return the component
				if (attemptedCast != nullptr)
				{
					return attemptedCast;
				}
			}

			// If we are here, we didn't find the component and have nothing to return
			return nullptr;
		}

	protected:
		template<typename T>
		inline std::shared_ptr<T> getResourcePointer(const YAML::Node& node, const std::string& resourceName)
		{
			YAML::Node resourceIdNode = node[resourceName];
			if (resourceIdNode == nullptr || !resourceIdNode.IsScalar())
			{
				return nullptr;
			}

			boost::uuids::uuid resourceId = resourceIdNode.as<boost::uuids::uuid>();

			return ObjectLibrary::getInstance().getResourceObjectPointer<T>(resourceId);
		}

		template<typename T>
		inline T getResource(YAML::Node const& node, std::string const& resourceName)
		{
			Resources::Resource* resource = getResource(node, resourceName);
			return static_cast<T>(resource);
		}

		inline Resources::Resource* getResource(YAML::Node const& node, std::string const& resourceName)
		{
			YAML::Node resourceNode = node[resourceName];

			if (resourceNode == nullptr)
			{
				return nullptr;
			}

			boost::uuids::uuid id = resourceNode.as<boost::uuids::uuid>();
			Resources::Resource * resource = ObjectLibrary::getInstance().getResource(id);
			return resource;
		}

		template<typename T>
		inline T* getResourceObject(boost::uuids::uuid const& resourceId)
		{
			return ObjectLibrary::getInstance().getResourceObject<T>(resourceId);
		}

		template<typename T>
		inline T* getResourceObject(YAML::Node const& node, std::string const& resourceName)
		{
			YAML::Node resourceIdNode = node[resourceName];
			if (resourceIdNode == nullptr || !resourceIdNode.IsScalar())
			{
				return nullptr;
			}

			boost::uuids::uuid resourceId = resourceIdNode.as<boost::uuids::uuid>();

			return getResourceObject<T>(resourceId);
		}

		template<typename T>
		inline std::vector<T> loadComponents(YAML::Node const& node, std::string const& componentCollectionName)
		{
			std::vector<T> objectArr = std::vector<T>();

			// Get the collection node
			YAML::Node componentIdCollectionNode = node[componentCollectionName];
			if (componentIdCollectionNode == nullptr || !componentIdCollectionNode.IsSequence())
			{
				return objectArr;
			}

			// Iterate through all component IDs in the collection
			for (size_t componentIndex = 0; componentIndex < componentIdCollectionNode.size(); componentIndex++)
			{
				boost::uuids::uuid id = componentIdCollectionNode[componentIndex].as<boost::uuids::uuid>();
				GameComponent* component = ObjectLibrary::getInstance().getComponent(id);
				if (component)
				{
					// Add it to the array if we found one
					objectArr.push_back((T)component);
				}
				else
				{
					// If no component was found, log the issue and continue on
					std::cout << "Unable to load the component with ID of '" << id << "' because it was not found in the ObjectLibrary.\n";
				}
			}

			// Serve up what we found
			return objectArr;
		}

	private:
		std::shared_ptr<GameObject> m_gameObject;
	};

}
