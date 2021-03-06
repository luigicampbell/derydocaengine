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
	bool SelfRegister<T>::s_isRegistered = GameComponentFactory::getInstance().registerGenerator(T::getClassName(), T::generateInstance) && TypeNameLookup::getInstace().registerType<T>(T::getClassName());

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

		template<typename T>
		inline std::shared_ptr<T> getComponent()
		{
			// Get the game object that this component belongs to
			auto gameObject = getGameObject();
			if (gameObject == nullptr)
			{
				return nullptr;
			}

			// Iterate through all of the components on this game object
			std::vector<std::shared_ptr<Components::GameComponent>> gameObjectComponents = getGameObject()->getComponents();
			for (std::vector<std::shared_ptr<Components::GameComponent>>::iterator it = gameObjectComponents.begin(); it != gameObjectComponents.end(); ++it)
			{
				// Attempt to cast this object to the type we want
				std::shared_ptr<Components::GameComponent> component = *it;
				auto attemptedCast = std::dynamic_pointer_cast<T>(component);

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

			return ObjectLibrary::getInstance().getResourceObjectPointer<T>(resourceId);
		}

		template<typename T>
		inline std::vector<std::shared_ptr<T>> loadComponents(const YAML::Node& node, const std::string& componentCollectionName)
		{
			std::vector<std::shared_ptr<T>> objectArr = std::vector<std::shared_ptr<T>>();

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
				auto component = ObjectLibrary::getInstance().getComponent(id);
				if (component)
				{
					// Add it to the array if we found one
					objectArr.push_back(std::static_pointer_cast<T>(component));
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
		std::weak_ptr<GameObject> m_gameObject;
	};

}
