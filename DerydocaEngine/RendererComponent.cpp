#include "RendererComponent.h"
#include "LightManager.h"

namespace DerydocaEngine::Components
{

	void RendererComponent::updateMesh()
	{
		if (m_dirtyComponents == MeshComponents::None)
		{
			return;
		}

		if (!m_mesh)
		{
			m_mesh = new Mesh();
		}

		if (m_dirtyComponents & MeshComponents::Positions)
		{
			m_vertices = generateVertices();
			m_numVertices = generateNumVertices();
		}
		if (m_dirtyComponents & MeshComponents::TexCoords)
		{
			m_texCoords = generateTexCoords();
		}
		if (m_dirtyComponents & MeshComponents::Colors)
		{
			m_vertexColors = generateVertexColors();
		}
		if (m_dirtyComponents & MeshComponents::Indices)
		{
			m_triangleIndices = generateTriangleIndices();
			m_numIndices = generateNumIndices();
		}
		if (m_dirtyComponents & MeshComponents::Tangents)
		{
			m_tangents = generateTangents();
		}
		if (m_dirtyComponents & MeshComponents::Bitangents)
		{
			m_bitangents = generateBitangents();
		}
		if (m_dirtyComponents & MeshComponents::Normals)
		{
			m_normals = generateNormals();
		}

		m_mesh->loadMeshComponentData(
			m_dirtyComponents,
			m_numVertices,
			m_vertices,
			m_tangents,
			m_bitangents,
			m_texCoords,
			m_normals,
			m_numIndices,
			m_triangleIndices,
			m_vertexColors
		);

		m_dirtyComponents = MeshComponents::None;
	}

	void RendererComponent::render(MatrixStack * const& matrixStack)
	{
		if (m_dirtyComponents != MeshComponents::None)
		{
			updateMesh();
		}

		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);
		LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), m_material->getShader());

		m_mesh->draw();

		m_material->unbind();
	}

	void RendererComponent::renderMesh(MatrixStack * const& matrixStack, Material * const& material, Projection const& projection, Transform * const& projectionTransform)
	{
		m_material->bind();
		m_material->getShader()->update(matrixStack, projection, projectionTransform);
		m_mesh->draw();
		m_material->unbind();
	}

}