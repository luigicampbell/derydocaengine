#include "MaterialFileSerializer.h"
#include <yaml-cpp\yaml.h>
#include "Material.h"
#include "Shader.h"
#include "CubemapResource.h"

namespace DerydocaEngine::FileSerializers {

	Files::FileType MaterialSerializer::getFileType()
	{
		return Files::FileType::MaterialFileType;
	}

	YAML::Node MaterialSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node materialResource;
		materialResource["Type"] = "Material";
		materialResource["ID"] = generateUuid();

		resources.push_back(materialResource);

		// Load the yaml file
		YAML::Node root = YAML::LoadFile(filePath);

		// Open the material parameters node
		YAML::Node materialParamNode = root["MaterialParameters"];
		if (materialParamNode && materialParamNode.IsSequence())
		{
			// Iterate through each parameter
			for (unsigned int i = 0; i < materialParamNode.size(); i++)
			{
				// Get the type of the parameter
				YAML::Node parameterTypeNode = materialParamNode[i]["Type"];
				if (!parameterTypeNode || !parameterTypeNode.IsScalar())
				{
					continue;
				}
				std::string parameterType = parameterTypeNode.as<std::string>();

				// If it is a cubemap, then create a cubemap resource
				if (parameterType == "Cubemap")
				{
					YAML::Node cubemapResource;
					cubemapResource["Type"] = "Cubemap";
					cubemapResource["Slot"] = i;
					cubemapResource["ID"] = generateUuid();

					resources.push_back(cubemapResource);
				}
			}
		}

		return resources;
	}

	Resources::Resource * MaterialSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		// Get the resource type
		YAML::Node resourceTypeNode = resourceNode["Type"];
		std::string resourceType;
		if (resourceTypeNode && resourceTypeNode.IsScalar())
		{
			resourceType = resourceTypeNode.as<std::string>();
		}
		else
		{
			resourceType = "Material";
		}

		// Create a material resource type
		if (resourceType == "Cubemap")
		{
			Resources::CubemapResource* r = new Resources::CubemapResource();
			r->setType(Resources::CubemapResourceType);
			if (resourceNode["Slot"])
			{
				r->setSlot(resourceNode["Slot"].as<int>());
			}
			return r;
		}
		else
		{
			Resources::Resource* r = new Resources::Resource();
			r->setType(Resources::MaterialResourceType);
			return r;
		}
	}

}
