#pragma once
#include "glm\glm.hpp"
#include "GameComponent.h"
#include "Color.h"

class Light : public GameComponent
{
public:
	/* Defines a light's type */
	enum LightType
	{
		/* A orthagonal light infinitely far away */
		Directional,
		/* A light projected in a frustum */
		Spotlight,
		/* A light radiating in all directions */
		Point
	};

	Light();
	~Light();

	void setLightType(LightType type) { m_lightType = type; }
	LightType getLightType() { return m_lightType; }

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	void setColor(Color* color)
	{
		m_color.r = color->r;
		m_color.g = color->g;
		m_color.b = color->b;
		m_color.a = color->a;
	}
	Color getColor() { return m_color; }
private:
	LightType m_lightType = Point;
	Color m_color = Color(1, 1, 1, 1);
};
