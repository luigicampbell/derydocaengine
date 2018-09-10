#pragma once
#include "GameComponent.h"
#include "Material.h"

class NoiseTexture : public GameComponent
{
public:
	GENINSTANCE(NoiseTexture);

	NoiseTexture();
	~NoiseTexture();

	virtual void init();
	virtual void deserialize(YAML::Node const& compNode);

	void generateNoiseTexture(float const& baseFreq, float const& persistence, bool const& periodic, bool const& seamless);
private:
	int m_width = 256;
	int m_height = 256;
	std::string m_textureName = "NoiseTexture";
	Material* m_material = nullptr;
	Texture* m_texture = nullptr;
	float m_baseFrequency = 4.0;
	float m_persistence = 1.0;
	bool m_periodic = false;
	bool m_seamless = false;
};

