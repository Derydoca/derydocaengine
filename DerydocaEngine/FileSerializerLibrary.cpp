#include "FileSerializerLibrary.h"

#include "MaterialFileSerializer.h"
#include "MeshFileSerializer.h"
#include "ImageFileSerializer.h"
#include "LevelFileSerializer.h"
#include "ShaderFileSerializer.h"
#include "BezierPatchMeshFileSerializer.h"
#include "FontFileSerializer.h"
#include "RasterFontFileSerializer.h"
#include "SpriteSheetFileSerializer.h"

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
	registerTypeSerializer(new ShaderFileSerializer());
	registerTypeSerializer(new BezierPatchMeshFileSerializer());
	registerTypeSerializer(new FontFileSerializer());
	registerTypeSerializer(new RasterFontFileSerializer());
	registerTypeSerializer(new SpriteSheetFileSerializer());
}

FileSerializerLibrary::~FileSerializerLibrary()
{
	// Delete all type serializers
	for (std::map<FileType, FileTypeSerializer*>::iterator it = m_typeSerializers.begin(); it != m_typeSerializers.end(); ++it)
	{
		delete(it->second);
	}
}
