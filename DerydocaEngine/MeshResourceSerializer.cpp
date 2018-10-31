#include "MeshResourceSerializer.h"
#include "MeshResource.h"
#include "Mesh.h"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include "MeshAdjacencyCalculator.h"

namespace DerydocaEngine::Resources::Serializers
{
	MeshResourceSerializer::MeshResourceSerializer()
	{
	}

	MeshResourceSerializer::~MeshResourceSerializer()
	{
	}

	void * MeshResourceSerializer::deserialize(Resource * const& resource)
	{
		return nullptr;
	}

	std::shared_ptr<void> MeshResourceSerializer::deserializePointer(Resource * const & resource)
	{
		MeshResource* mr = (MeshResource*)resource;

		std::shared_ptr<Animation::Skeleton> skeleton;
		if (mr->hasSkeleton())
		{
			skeleton = ObjectLibrary::getInstance().getResourceObjectPointer<Animation::Skeleton>(mr->getSkeletonId());
		}
		
		const aiScene* aiScene = aiImportFile(resource->getSourceFilePath().c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		int uvIndex = 0;

		// Find the mesh index
		unsigned int meshIndex = 0;
		for (unsigned int i = 0; i < aiScene->mNumMeshes; i++)
		{
			std::string meshName = aiScene->mMeshes[i]->mName.data;
			if (meshName == mr->getMeshName())
			{
				meshIndex = i;
				break;
			}
		}

		aiMesh* mesh = aiScene->mMeshes[meshIndex];

		unsigned int m_numVertices = mesh->mNumVertices;
		glm::vec3* m_positions = 0;
		unsigned int m_numIndices = 0;
		glm::vec3* m_normals = nullptr;
		glm::vec2* m_texCoords = nullptr;
		unsigned int* m_indices = nullptr;
		glm::vec3* m_tangents = nullptr;
		glm::vec3* m_bitangents = nullptr;
		unsigned int* m_boneIndices = nullptr;
		float* m_boneWeights = nullptr;
		Rendering::MeshFlags m_flags = mr->getFlags();

		ProcessMeshData(mesh, m_positions, m_numVertices, uvIndex, m_texCoords, m_normals, m_flags, m_numIndices, m_indices, m_tangents, m_bitangents);

		if (mesh->mNumBones > 0)
		{
			ProcessBoneData(mesh, m_boneIndices, m_boneWeights, skeleton);
		}

		std::shared_ptr<Rendering::Mesh> m = std::make_shared<Rendering::Mesh>(
				m_numVertices,
				m_numIndices,
				m_positions,
				m_indices,
				m_normals,
				m_texCoords,
				m_tangents,
				m_bitangents,
				nullptr,
				m_boneIndices,
				m_boneWeights);
		m->setSkeleton(skeleton);

		m->setFlags(mr->getFlags());

		return std::static_pointer_cast<void>(m);
	}

	void MeshResourceSerializer::ProcessMeshData(aiMesh * &mesh, glm::vec3 * &m_positions, unsigned int m_numVertices, int uvIndex, glm::vec2 * &m_texCoords, glm::vec3 * &m_normals, DerydocaEngine::Rendering::MeshFlags m_flags, unsigned int &m_numIndices, unsigned int * &m_indices, glm::vec3 * &m_tangents, glm::vec3 * &m_bitangents)
	{
		if (mesh->HasPositions())
		{
			m_positions = new glm::vec3[m_numVertices];
			for (unsigned int i = 0; i < m_numVertices; i++)
			{
				m_positions[i] = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			}
		}

		if (mesh->HasTextureCoords(uvIndex))
		{
			m_texCoords = new glm::vec2[m_numVertices];
			for (unsigned int i = 0; i < m_numVertices; i++)
			{
				m_texCoords[i] = glm::vec2(mesh->mTextureCoords[uvIndex][i].x, mesh->mTextureCoords[uvIndex][i].y);
			}
		}

		if (mesh->HasNormals())
		{
			m_normals = new glm::vec3[m_numVertices];
			for (unsigned int i = 0; i < m_numVertices; i++)
			{
				m_normals[i] = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
		}

		if (mesh->HasFaces())
		{
			if (m_flags & Rendering::MeshFlags::load_adjacent)
			{
				Ext::MeshAdjacencyCalculator mac;
				m_numIndices = mesh->mNumFaces * 3 * 2;
				m_indices = new unsigned int[m_numIndices];
				mac.buildAdjacencyList(mesh, m_indices);
			}
			else
			{
				m_numIndices = mesh->mNumFaces * 3;
				m_indices = new unsigned int[m_numIndices];
				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					m_indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
					m_indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
					m_indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
				}
			}
		}

		if (mesh->HasTangentsAndBitangents())
		{
			m_tangents = new glm::vec3[m_numVertices];
			for (unsigned int i = 0; i < m_numVertices; i++)
			{
				m_tangents[i] = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			}

			m_bitangents = new glm::vec3[m_numVertices];
			for (unsigned int i = 0; i < m_numVertices; i++)
			{
				m_bitangents[i] = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
			}
		}
	}

	void MeshResourceSerializer::ProcessBoneData(aiMesh * mesh, unsigned int * &m_boneIndices, float * &m_boneWeights, const std::shared_ptr<Animation::Skeleton>& skeleton)
	{
		// Create buffers that will store the bone indices and bone weights
		unsigned int numTotalBoneElements = mesh->mNumVertices * Rendering::Mesh::MAX_BONES;
		m_boneIndices = new unsigned int[numTotalBoneElements];
		for (unsigned int i = 0; i < numTotalBoneElements; i++)
		{
			m_boneIndices[i] = -1;
		}
		m_boneWeights = new float[numTotalBoneElements] { };

		// For each bone in the source mesh file
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			// Get the bone
			aiBone* bone = mesh->mBones[i];
			int boneIndex = skeleton->getBoneID(bone->mName.data);

			// For each weight associated with the bone
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				// Extract the weight data
				aiVertexWeight vertWeight = bone->mWeights[w];
				int vertexIndex = vertWeight.mVertexId;

				// Pull data from the weights and load it into the bone weight map
				unsigned int bufferOffset = vertWeight.mVertexId * Rendering::Mesh::MAX_BONES;

				// For each vertex associated with the weight
				for (int weightIndex = 0; weightIndex < Rendering::Mesh::MAX_BONES; weightIndex++)
				{
					if (m_boneWeights[bufferOffset + weightIndex] <= 0)
					{
						m_boneWeights[bufferOffset + weightIndex] = vertWeight.mWeight;
						m_boneIndices[bufferOffset + weightIndex] = boneIndex;

						break;
					}
				}
			}
		}
	}

	ResourceType MeshResourceSerializer::getResourceType()
	{
		return ResourceType::MeshResourceType;
	}

}
