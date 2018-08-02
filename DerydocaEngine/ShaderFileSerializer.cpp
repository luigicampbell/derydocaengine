#include "ShaderFileSerializer.h"
#include "ShaderResource.h"
#include "boost/filesystem.hpp"
#include "boost/filesystem/convenience.hpp"
#include "ShaderLibrary.h"

using namespace boost::filesystem;

YAML::Node ShaderFileSerializer::generateResourceNodes(std::string filePath)
{
	YAML::Node resources;

	YAML::Node shaderResource;
	shaderResource["ID"] = generateUuid();

	resources.push_back(shaderResource);

	return resources;
}

FileType ShaderFileSerializer::getFileType()
{
	return FileType::ShaderFileType;
}

Resource * ShaderFileSerializer::loadResourceFromMeta(YAML::Node resourceNode)
{
	ShaderResource* resource = new ShaderResource();

	resource->setType(ShaderResourceType);

	return resource;
}

void ShaderFileSerializer::postLoadInitialize(Resource* resource)
{
	ShaderResource* shaderResource = (ShaderResource*)resource;
	path vertexShaderPath(shaderResource->getVertexShaderLocation());

	// Get the path that this exists for
	path shaderDirectory = vertexShaderPath.parent_path();

	// Remove the extension from the file name so we can append to all other shader types
	path rawShaderName = change_extension(vertexShaderPath, "");
	shaderResource->setRawShaderName(rawShaderName.string());

	// If a fragment shader exists
	path fragmentShaderPath = rawShaderName.append(".fs");
	if (exists(fragmentShaderPath))
	{
		// Set the path to the fragment shader
		shaderResource->setFragmentShaderSource(fragmentShaderPath.string());
	}

	// If a geometry shader exists
	path geometryShaderPath = rawShaderName.append(".gs");
	if (exists(geometryShaderPath))
	{
		// Set the path to the geometry shader
		shaderResource->setGeometryShaderSource(geometryShaderPath.string());
	}

	// Store the shader name in the library
	ShaderLibrary::getInstance().registerShaderName(shaderResource->getRawShaderName(), shaderResource->getId());
}