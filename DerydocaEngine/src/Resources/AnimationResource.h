#pragma once
#include "Resources\Resource.h"

namespace DerydocaEngine::Resources
{

	struct AnimationResource : public Resource
	{
	public:
		AnimationResource() :
			m_name()
		{}

		void setName(std::string const& animationName) { m_name = animationName; }
		std::string getName() { return m_name; }
	private:
		std::string m_name;
	};

}
