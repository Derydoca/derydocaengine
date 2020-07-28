#include "EnginePch.h"
#include "Resources\Serializers\MaterialResourceSerializer.h"
#include "Rendering\Material.h"
#include "Rendering\Texture.h"
#include "Resources\Resource.h"
#include "Rendering\ShaderLibrary.h"
#include "Files\FileUtils.h"
#include "AssetData\MaterialData.h"
#include "ObjectLibrary.h"
#include "Rendering\Shader.h"
#include "Serialization\CerealGlmTypes.h"

namespace DerydocaEngine::Resources::Serializers
{
    std::shared_ptr<void> MaterialResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
    {
        auto data = Files::Utils::ReadFromDisk<AssetData::MaterialData>(resource->getSourceFilePath());
        auto material = std::make_shared<Rendering::Material>();

        auto shader = ObjectLibrary::getInstance().getResource<Resources::ShaderResource>(data.ShaderId);
        material->setShader(shader);

        for (auto prop : data.Textures)
        {
            if (prop.Value)
            {
                material->setTexture(prop.Name, prop.Value);
            }
        }

        for (auto prop : data.Bools) material->setBool(prop.Name, prop.Value);
        for (auto prop : data.Ints) material->setInt(prop.Name, prop.Value);
        for (auto prop : data.Floats) material->setFloat(prop.Name, prop.Value);
        for (auto prop : data.FloatArrays) material->setFloatArray(prop.Name, prop.Value);
        for (auto prop : data.Float3s) material->setVec3(prop.Name, prop.Value);
        for (auto prop : data.Float4s) material->setVec4(prop.Name, prop.Value);
        for (auto prop : data.Float3x3s) material->setMat3(prop.Name, prop.Value);
        for (auto prop : data.Float4x4s) material->setMat4(prop.Name, prop.Value);
        for (auto prop : data.Float4x4Arrays) material->setMat4Array(prop.Name, prop.Value);
        for (auto prop : data.SubroutineValues) material->setSubroutine(prop.Program, prop.Value);

        return material;
    }

}
