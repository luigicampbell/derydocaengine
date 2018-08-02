#include "NoiseTexture.h"
#include <glm/gtc/noise.hpp>
#include "MeshRenderer.h"

using namespace glm;

NoiseTexture::NoiseTexture()
{
	m_texture = new Texture();
}

NoiseTexture::~NoiseTexture()
{
	delete m_texture;
}

void NoiseTexture::init()
{
	MeshRenderer* mr = getComponent<MeshRenderer>();
	if (mr)
	{
		m_material = mr->getMaterial();
	}
	else
	{
		Camera* cam = getComponent<Camera>();
		if (cam)
		{
			m_material = cam->getPostProcessMaterial();
		}
	}

	generateNoiseTexture(m_baseFrequency, m_persistence, m_periodic, m_seamless);
}

void NoiseTexture::deserialize(YAML::Node compNode)
{
	YAML::Node widthNode = compNode["width"];
	if (widthNode)
	{
		m_width = widthNode.as<int>();
	}

	YAML::Node heightNode = compNode["height"];
	if (heightNode)
	{
		m_height = heightNode.as<int>();
	}

	YAML::Node textureNameNode = compNode["textureName"];
	if (textureNameNode)
	{
		m_textureName = textureNameNode.as<string>();
	}

	YAML::Node baseFrequencyNode = compNode["baseFrequency"];
	if (baseFrequencyNode)
	{
		m_baseFrequency = baseFrequencyNode.as<float>();
	}

	YAML::Node persistenceNode = compNode["persistence"];
	if (persistenceNode)
	{
		m_persistence = persistenceNode.as<float>();
	}

	YAML::Node periodicNode = compNode["periodic"];
	if (periodicNode)
	{
		m_periodic = periodicNode.as<bool>();
	}

	YAML::Node seamlessNode = compNode["seamless"];
	if (seamlessNode)
	{
		m_seamless = seamlessNode.as<bool>();
	}
}

void NoiseTexture::generateNoiseTexture(float baseFreq, float persistence, bool periodic, bool seamless)
{
	GLubyte* textureData = new GLubyte[m_width * m_height * 4];

	float xRange = 1.0f;
	float yRange = 1.0f;
	float xFactor = xRange / (m_width - 1);
	float yFactor = yRange / (m_height - 1);

	for (int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			float x = xFactor * col;
			float y = yFactor * row;
			float sum = 0.0f;
			float freq = baseFreq;
			float persist = persistence;
			for (int oct = 0; oct < 4; oct++) {

				float val = 0.0f;
				if (seamless)
				{
					float a, b, c, d;

					vec2 aPos = vec2(x, y) * freq;
					vec2 bPos = vec2(x + xRange, y) * freq;
					vec2 cPos = vec2(x, y + yRange) * freq;
					vec2 dPos = vec2(x + xRange, y + yRange) * freq;

					if (periodic)
					{
						a = perlin(aPos);
						b = perlin(bPos);
						c = perlin(cPos);
						d = perlin(dPos);
					}
					else
					{
						a = perlin(aPos, vec2(freq));
						b = perlin(bPos, vec2(freq));
						c = perlin(cPos, vec2(freq));
						d = perlin(dPos, vec2(freq));
					}

					a *= persist;
					b *= persist;
					c *= persist;
					d *= persist;

					float xmix = 1.0 - x / xRange;
					float ymix = 1.0 - y / yRange;
					float x1 = mix(a, b, xmix);
					float x2 = mix(c, d, xmix);

					val = mix(x1, x2, ymix);
				}
				else
				{
					glm::vec2 p(x * freq, y * freq);
					if (periodic)
					{
						val = glm::perlin(p, vec2(freq));
					}
					else
					{
						val = glm::perlin(p);
					}

					val *= persist;
				}

				sum += val;

				float result = (sum + 1.0f) / 2.0f;

				// Clamp strictly between 0 and 1
				result = result > 1.0f ? 1.0f : result;
				result = result < 0.0f ? 0.0f : result;

				// Store in texture
				textureData[((row * m_width + col) * 4) + oct] = (GLubyte)(result * 255.0f);
				freq *= 2.0f;
				persist *= persistence;
			}
		}
	}

	TextureParameters texParams;
	texParams.setWrapModeS(TextureWrapMode::REPEAT);
	texParams.setWrapModeT(TextureWrapMode::REPEAT);
	m_texture->updateBuffer(textureData, m_width, m_height, GL_RGBA, &texParams);

	m_material->setTexture(m_textureName, m_texture);

	delete[] textureData;
}