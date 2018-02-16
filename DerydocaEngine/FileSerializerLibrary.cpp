#include "FileSerializerLibrary.h"

#include "MaterialFileSerializer.h"
#include "MeshFileSerializer.h"
#include "ImageFileSerializer.h"
#include "LevelFileSerializer.h"

FileTypeSerializer * FileSerializerLibrary::getTypeSerializer(std::string sourceFilePath)
{
	// Determine the resource type from the file path
	FileType type = pathToFileType(sourceFilePath);

	// Return the serializer associated to that type
	return getTypeSerializer(type);
}

FileTypeSerializer * FileSerializerLibrary::getTypeSerializer(FileType type)
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

void FileSerializerLibrary::registerTypeSerializer(FileTypeSerializer * serializer)
{
	// Add this serializer to the list of serializers
	m_typeSerializers.insert(std::pair<FileType, FileTypeSerializer*>(serializer->getFileType(), serializer));
}

FileSerializerLibrary::FileSerializerLibrary()
{
	// Register all type serializers
	registerTypeSerializer(new MaterialSerializer());
	registerTypeSerializer(new MeshSerializer());
	registerTypeSerializer(new ImageFileSerializer());
	registerTypeSerializer(new LevelFileSerializer());
}

FileSerializerLibrary::~FileSerializerLibrary()
{
	// Delete all type serializers
	for (std::map<FileType, FileTypeSerializer*>::iterator it = m_typeSerializers.begin(); it != m_typeSerializers.end(); ++it)
	{
		delete(it->second);
	}
}
