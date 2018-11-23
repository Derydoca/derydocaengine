#pragma once
#include <map>
#include "FileTypeSerializer.h"
#include "ResourceType.h"

namespace DerydocaEngine::Files::Serializers {

	class FileSerializerLibrary
	{
	public:
		static FileSerializerLibrary& getInstance() {
			static FileSerializerLibrary instance;
			return instance;
		}

		FileTypeSerializer * getTypeSerializer(std::string const& sourceFilePath);
		FileTypeSerializer * getTypeSerializer(Files::FileType const& type);

		void operator=(FileSerializerLibrary const&) = delete;
	private:
		FileSerializerLibrary();
		~FileSerializerLibrary();
		FileSerializerLibrary(FileSerializerLibrary const&);

		void registerTypeSerializer(FileTypeSerializer* const& serializer);

		std::map<Files::FileType, FileTypeSerializer*> m_typeSerializers;
	};

}
