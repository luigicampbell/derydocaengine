#include "ShaderSubroutineSwitcher.h"

#include <gl\glew.h>
#include <iostream>
#include "Components\MeshRenderer.h"
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	void ShaderSubroutineSwitcher::init()
	{
		setSubroutine(GL_VERTEX_SHADER, m_subroutineName);
	}

	void ShaderSubroutineSwitcher::deserialize(YAML::Node const& compNode)
	{
		m_subroutineName = compNode["SubroutineName"].as<std::string>();
	}

	void ShaderSubroutineSwitcher::setSubroutine(unsigned int const& program, std::string const& subroutineName)
	{
		if (subroutineName.empty())
		{
			std::cout << "Unable to set the subroutine for this component because a subroutine name was not supplied.\n";
			return;
		}

		// Get the shader from the attached mesh renderer
		Components::MeshRenderer* mr = getComponent<Components::MeshRenderer>();
		if (mr == nullptr)
		{
			std::cout << "No mesh renderer found for ShaderSubroutineSwitcher object.\n";
			return;
		}
		// Get the shader attached to the mesh renderer
		Rendering::Material* material = mr->getMaterial();
		Rendering::Shader* shader = material->getShader();
		if (shader == nullptr)
		{
			std::cout << "Unable to set the subroutine for this component because it does not have a shader assigned to it.\n";
			return;
		}

		// Ask the shader for the index of the desired subroutine
		GLuint subroutineIndex = shader->getSubroutineIndex(GL_VERTEX_SHADER, subroutineName);

		// Tell the shader to use this subroutine
		material->setSubroutine(program, subroutineIndex);
	}

}
