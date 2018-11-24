#include "EdgeDetectionFilter.h"
#include "Components\Camera.h"
#include "Rendering\RenderTexture.h"
#include "Components\MeshRenderer.h"
#include "Rendering\Shader.h"
#include <iostream>

namespace DerydocaEngine::Ext
{

	void EdgeDetectionFilter::init()
	{
		m_postProcessCamera = getComponent<Components::Camera>();
		if (m_postProcessCamera == nullptr)
		{
			std::cout << "No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.\n";
			return;
		}

		updateShader();
	}

	void EdgeDetectionFilter::deserialize(YAML::Node const& compNode)
	{
		YAML::Node edgeThresholdNode = compNode["edgeThreshold"];
		if (edgeThresholdNode)
		{
			m_edgeThreshold = edgeThresholdNode.as<float>();
		}

	}

	void EdgeDetectionFilter::update(float const& deltaTime)
	{
		updateShader();
	}

	void EdgeDetectionFilter::updateShader()
	{
		Rendering::Shader* shader = m_postProcessCamera->getPostProcessShader();
		if (shader == nullptr)
		{
			return;
		}

		shader->setFloat("EdgeThreshold", m_edgeThreshold);
	}

}