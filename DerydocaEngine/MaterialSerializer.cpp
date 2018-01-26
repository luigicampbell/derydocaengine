#include "MaterialSerializer.h"
#include <yaml-cpp\yaml.h>
#include "Material.h"
#include "Shader.h"

MaterialSerializer::MaterialSerializer()
{
}

MaterialSerializer::~MaterialSerializer()
{
}

ResourceType MaterialSerializer::getResourceType()
{
	return ResourceType::MaterialResource;
}

void * MaterialSerializer::deserialize(Resource* resource)
{
	YAML::Node root = YAML::LoadFile(resource->getSourceFilePath());
	std::string shaderPath = root["Shader"].as<std::string>();
	Shader* shader = new Shader(shaderPath);
	Material* material = new Material();
	material->setShader(shader);
	YAML::Node parameters = root["MaterialParameters"];
	for (int i = 0; i < parameters.size(); i++)
	{
		std::string paramType = parameters[i]["Type"].as<std::string>();
		if (paramType == "Texture")
		{
			int slot = parameters[i]["Slot"].as<int>();
			std::string texturePath = parameters[i]["Path"].as<std::string>();
			Texture* texture = new Texture(texturePath);
			material->setTextureSlot(slot, texture);
		}
	}
	return material;
}