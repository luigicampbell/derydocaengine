#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct SkeletonResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(SkeletonResource);

		SkeletonResource() :
			m_name()
		{}

		void setName(std::string const& animationName) { m_name = m_name; }
		std::string getName() { return m_name; }
	private:
		std::string m_name;
	};

}
