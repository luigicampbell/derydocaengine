#include "RasterFontFileSerializer.h"

RasterFontFileSerializer::RasterFontFileSerializer()
{
}

RasterFontFileSerializer::~RasterFontFileSerializer()
{
}

YAML::Node RasterFontFileSerializer::generateResourceNodes(std::string filePath)
{
	YAML::Node resources;

	YAML::Node levelResource;
	levelResource["ID"] = generateUuid();

	resources.push_back(levelResource);

	return resources;
}

FileType RasterFontFileSerializer::getFileType()
{
	return FileType::RasterFontType;
}

Resource * RasterFontFileSerializer::loadResourceFromMeta(YAML::Node resourceNode)
{
	Resource* r = new Resource();
	r->setType(RasterFontResourceType);
	return r;
}