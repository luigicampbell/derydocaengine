#pragma once
#include "Files\Serializers\FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class ShaderFileSerializer : public FileTypeSerializer
	{
	public:
		ShaderFileSerializer() {};
		~ShaderFileSerializer() {};

		YAML::Node generateResourceNodes(std::string const& filePath);
		Files::FileType getFileType();
		std::shared_ptr<Resources::Resource> loadResourceFromMeta(YAML::Node const& resourceNode);
		virtual void postLoadInitialize(std::shared_ptr<Resources::Resource> resource);
	};

}
