#pragma once
#include <glm\glm.hpp>
#include "Components\GameComponent.h"
#include "Rendering\Texture.h"
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	class ParticleSystem : public Components::GameComponent, Components::SelfRegister<ParticleSystem>
	{
	public:
		GENINSTANCE(ParticleSystem);

		ParticleSystem();
		~ParticleSystem();

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
		virtual void render(std::shared_ptr<Rendering::MatrixStack> const matrixStack);
	private:
		unsigned int m_numParticles;
		float m_size2;
		unsigned int m_vao;
		unsigned int m_vertexArrayBuffers[1];
		glm::vec3 m_volumeSize;
		glm::vec3* m_particleLocations;
		Rendering::Texture* m_texture;
		std::shared_ptr<Rendering::Material> m_material;
	};

}
