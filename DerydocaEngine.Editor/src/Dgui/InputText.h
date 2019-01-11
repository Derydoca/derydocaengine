#pragma once

namespace DerydocaEngine::Dgui
{
	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback ChainCallback;
		void* ChainCallbackUserData;
	};

	static int InputTextCallback(ImGuiInputTextCallbackData* data);

	bool InputText(
		std::string label,
		std::string& str,
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_None,
		ImGuiInputTextCallback callback = nullptr,
		void* user_data = nullptr
	);

}