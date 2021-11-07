#include "EnginePch.h"
#include "Files\Serializers\FileSerializerLibrary.h"

#include "Files\Serializers\CubemapFileSerializer.h"
#include "Files\Serializers\FileTypeSerializer.h"
#include "Files\Serializers\FontFileSerializer.h"
#include "Files\Serializers\ImageFileSerializer.h"
#include "Files\Serializers\LevelFileSerializer.h"
#include "Files\Serializers\MaterialFileSerializer.h"
#include "Files\Serializers\MeshFileSerializer.h"
#include "Files\Serializers\RasterFontFileSerializer.h"
#include "Files\Serializers\ShaderFileSerializer.h"
#include "Files\Serializers\SpriteSheetFileSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	FileTypeSerializer * FileSerializerLibrary::getTypeSerializer(std::string const& sourceFilePath)
	{
		// Determine the resource type from the file path
		Files::FileType type = Files::pathToFileType(sourceFilePath);

		// Return the serializer associated to that type
		return getTypeSerializer(type);
	}

	FileTypeSerializer * FileSerializerLibrary::getTypeSerializer(Files::FileType const& type)
	{
		// Return the associated serializer for the resource type specified
		auto search = m_typeSerializers.find(type);
		if (search != m_typeSerializers.end())
		{
			return search->second;
		}
		else
		{
			return nullptr;
		}
	}

	void FileSerializerLibrary::registerTypeSerializer(FileTypeSerializer * const& serializer)
	{
		// Add this serializer to the list of serializers
		m_typeSerializers.insert(std::pair<Files::FileType, FileTypeSerializer*>(serializer->getFileType(), serializer));
	}

	FileSerializerLibrary::FileSerializerLibrary()
	{
		// Register all type serializers
		registerTypeSerializer(new MaterialSerializer());
		registerTypeSerializer(new MeshSerializer());
		registerTypeSerializer(new ImageFileSerializer());
		registerTypeSerializer(new LevelFileSerializer());
		registerTypeSerializer(new ShaderFileSerializer());
		registerTypeSerializer(new FontFileSerializer());
		registerTypeSerializer(new RasterFontFileSerializer());
		registerTypeSerializer(new SpriteSheetFileSerializer());
		registerTypeSerializer(new CubemapSerializer());
	}

	FileSerializerLibrary::~FileSerializerLibrary()
	{
		// Delete all type serializers
		for (std::map<Files::FileType, FileTypeSerializer*>::iterator it = m_typeSerializers.begin(); it != m_typeSerializers.end(); ++it)
		{
			delete(it->second);
		}
	}

}
