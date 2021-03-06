#pragma once
#include <map>
#include <memory>
#include <string>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include "Resources\Resource.h"
#include "Resources\ResourceTreeNode.h"

namespace DerydocaEngine::Components {
	class GameComponent;
}

namespace DerydocaEngine
{

	class ObjectLibrary
	{
	public:
		static ObjectLibrary& getInstance()
		{
			static ObjectLibrary instance;
			return instance;
		}
		void operator=(ObjectLibrary const&) = delete;

		void loadEngineResources(const boost::filesystem::path& path);
		void loadProjectResources(const boost::filesystem::path& path);
		std::string getMetaExtension() const { return m_metaExtension; }
		std::shared_ptr<Resources::ResourceTreeNode> getRootResourceTreeNode() const { return m_projectResourceRoot; }
		std::shared_ptr<Resources::Resource> getResource(std::string const& uuidString);
		std::shared_ptr<Resources::Resource> getResource(boost::uuids::uuid const& uuid);
		std::shared_ptr<Components::GameComponent> getComponent(boost::uuids::uuid const& id);
		std::shared_ptr<Resources::Resource> getMetaFile(std::string const& sourceFilePath);
		void updateMetaFilesDirectory(const boost::filesystem::path& padirectoryth);
		void updateMetaFiles(std::string const& file);
		void loadDirectory(const boost::filesystem::path& path);
		void loadFile(std::string const& sourceFilePath);

		void registerComponent(boost::uuids::uuid const& id, std::shared_ptr<Components::GameComponent> component);

		template<class GameComponentClass>
		std::shared_ptr<GameComponentClass> getComponent(boost::uuids::uuid const& id)
		{
			auto component = getComponent(id);
			return std::static_pointer_cast<GameComponentClass>(component);
		}

		template<class resourceType>
		std::shared_ptr<resourceType> getResource(std::string const& uuidString)
		{
			boost::uuids::string_generator gen;
			boost::uuids::uuid uuid = gen(uuidString);
			std::shared_ptr<Resources::Resource> resource = getResource(uuid);
			return std::static_pointer_cast<resourceType>(resource);
		}

		template<class resourceType>
		std::shared_ptr<resourceType> getResource(const boost::uuids::uuid& uuid)
		{
			std::shared_ptr<Resources::Resource> resource = getResource(uuid);
			return std::static_pointer_cast<resourceType>(resource);
		}

		template<class resourceObjectType>
		std::shared_ptr<resourceObjectType> getResourceObjectPointer(boost::uuids::uuid const& id)
		{
			std::shared_ptr<Resources::Resource> resource = getResource(id);
			if (resource == nullptr)
			{
				return nullptr;
			}

			std::shared_ptr<resourceObjectType> pointer = std::static_pointer_cast<resourceObjectType>(resource->getResourceObjectPointer());
			return pointer;
		}
	private:
		ObjectLibrary();
		~ObjectLibrary() {}
		ObjectLibrary(ObjectLibrary const&) {}

		bool createMetaFile(std::string const& sourceFilePath, std::string const& metaFilePath);
		void registerResource(std::shared_ptr<Resources::Resource> resource);
		std::shared_ptr<Resources::ResourceTreeNode> getResourceTreeNode(const std::string& resourcePath);
		void loadResourceTree();

		const std::string m_metaExtension = ".derymeta";
		std::map<boost::uuids::uuid, std::shared_ptr<Resources::Resource>> m_resources;
		std::map<boost::uuids::uuid, std::shared_ptr<Components::GameComponent>> m_sceneComponents;
		std::shared_ptr<Resources::ResourceTreeNode> m_projectResourceRoot;
	};

}
