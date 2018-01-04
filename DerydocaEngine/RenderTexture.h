#pragma once
#include "GL\glew.h"
#include "Texture.h"

class RenderTexture : public Texture
{
public:
	RenderTexture();
	RenderTexture(int width, int height);
	~RenderTexture();

	void bind(unsigned int unit);
	void bindAsRenderTexture();
	float getAspectRatio();
	void initializeTexture(int width, int height);
private:
	GLuint m_framebuffer;
	GLuint m_depthbuffer;

	GLenum m_drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
};