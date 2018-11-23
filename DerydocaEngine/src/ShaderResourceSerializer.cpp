#include "ShaderResourceSerializer.h"

#include "Shader.h"
#include "ShaderLibrary.h"
#include "ShaderResource.h"

namespace DerydocaEngine::Resources::Serializers
{

	void * ShaderResourceSerializer::deserialize(Resource * const& resource)
	{
		ShaderResource* shaderResource = (ShaderResource*)resource;
		Rendering::Shader* shader = new Rendering::Shader(shaderResource->getRawShaderName());
		return shader;
	}

	ResourceType ShaderResourceSerializer::getResourceType()
	{
		return ResourceType::ShaderResourceType;
	}

}