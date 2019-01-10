#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"

namespace DerydocaEngine::Editor::Inspector
{

	template <typename T>
	class RegisterInspectorRenderer
	{
	protected:
		static bool s_isRegistered;
	};

	template <typename T>
	bool RegisterInspectorRenderer<T>::s_isRegistered = DerydocaEngine::Editor::Inspector::InspectorRendererFactory::getInstance().registerRenderer<T>(T::generateInstance());

#define REGISTER(TYPE) \
	static InspectorRenderer generateInstance() { return static_cast<DerydocaEngine::Editor::Inspector::InspectorRenderer>(TYPE>()); }\
	static std::string getName() { return #TYPE; }\
	void __forceRegistration() { s_isRegistered; };

	class InspectorRenderer
	{
	public:
		virtual void render(std::shared_ptr<Object> object) = 0;
	};

}
