#pragma once

#include "Components\GameComponent.h"
#include "Rendering\RenderTexture.h"
#include "Components\MeshRenderer.h"

namespace DerydocaEngine::Ext
{

	class EdgeDetectionFilter : public Components::GameComponent, Components::SelfRegister<EdgeDetectionFilter>
	{
	public:
		GENINSTANCE(EdgeDetectionFilter);

		EdgeDetectionFilter() {}
		~EdgeDetectionFilter() {}

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void update(const float deltaTime);
	private:
		void updateShader();

		float m_edgeThreshold;
		std::shared_ptr<Components::Camera> m_postProcessCamera;
	};

}
