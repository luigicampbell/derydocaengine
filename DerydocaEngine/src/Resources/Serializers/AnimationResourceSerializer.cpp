#include "EnginePch.h"
#include "Resources\Serializers\AnimationResourceSerializer.h"

#include "assimp\Importer.hpp"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "Animation\AnimationData.h"
#include "Resources\AnimationResource.h"
#include "Helpers\AssimpUtils.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<void> AnimationResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		Assimp::Importer importer;

		auto ar = std::static_pointer_cast<AnimationResource>(resource);
		const aiScene* scene = importer.ReadFile(resource->getSourceFilePath().c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		unsigned int animIndex = 0;
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			std::string animName = scene->mAnimations[i]->mName.data;
			if (animName == ar->getName())
			{
				animIndex = i;
				break;
			}
		}

		return Helpers::AssimpUtils::getAnimation(scene, animIndex);
	}

}
