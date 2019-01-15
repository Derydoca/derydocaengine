#pragma once
#include "Editor\Inspector\InspectorRendererFactory.h"

namespace DerydocaEngine::Editor::Inspector
{

	template <typename WindowType, typename ObjectType>
	class RegisterInspectorRenderer
	{
	protected:
		static bool s_isRegistered;
	};

	template <typename WindowType, typename ObjectType>
	bool RegisterInspectorRenderer<WindowType, ObjectType>::s_isRegistered = Editor::Inspector::InspectorRendererFactory::getInstance().registerRenderer<ObjectType>(WindowType::generateInstance());

#define REGISTER_INSPECTOR(TYPE) \
	public:\
	static std::shared_ptr<Editor::Inspector::InspectorRenderer> generateInstance() { return std::static_pointer_cast<Editor::Inspector::InspectorRenderer>(std::make_shared<TYPE>()); }\
	static std::string getName() { return #TYPE; }\
	void __forceRegistration() { s_isRegistered; };\
	private:\

	class InspectorRenderer
	{
	public:
		virtual void render(std::shared_ptr<Object> object) = 0;
	};

}
