#include "EnginePch.h"
#include "Rendering\Material.h"

#include "Rendering\Shader.h"
#include "Rendering\ShaderLibrary.h"
#include "Rendering\Texture.h"
#include "Color.h"

namespace DerydocaEngine::Rendering
{

	Material::Material() :
		m_shader(),
		m_texture(),
		m_boolValues(),
		m_intValues(),
		m_textures(),
		m_floatValues(),
		m_vec3Values(),
		m_vec4Values(),
		m_mat3Values(),
		m_mat4Values(),
		m_mat4ArrayValues()
	{
	}

	Material::~Material()
	{
	}

	void Material::setShader(std::shared_ptr<Resources::ShaderResource> shader)
	{
		if (!shader)
		{
			D_LOG_ERROR("Unable to set a shader because the shader reference is null.");
			m_shader = Rendering::ShaderLibrary::getInstance().getErrorShader();
			return;
		}

		auto shaderReference = std::static_pointer_cast<Rendering::Shader>(shader->getResourceObjectPointer());
		if (!shaderReference->isValid())
		{
			D_LOG_ERROR("Unable to set a shader because it is invalid.\nVert:{}\nFrag:{}", shaderReference->GetVertexShaderPath(), shaderReference->GetFragmentShaderPath());
			m_shader = Rendering::ShaderLibrary::getInstance().getErrorShader();
			return;
		}
		m_shader.Set(shader);
	}

	void Material::setShader(ResourceRef<Resources::ShaderResource> shader)
	{
		if (!shader)
		{
			D_LOG_ERROR("Unable to set a shader because the shader reference is null.");
			m_shader = Rendering::ShaderLibrary::getInstance().getErrorShader();
			return;
		}

		auto shaderReference = shader.As<Rendering::Shader>();
		if (!shaderReference->isValid())
		{
			D_LOG_ERROR("Unable to set a shader because it is invalid.\nVert:{}\nFrag:{}", shaderReference->GetVertexShaderPath(), shaderReference->GetFragmentShaderPath());
			m_shader = Rendering::ShaderLibrary::getInstance().getErrorShader();
			return;
		}
		m_shader = shader;
	}

	void Material::bind() const
	{
		assert(m_shader);

		auto shader = m_shader.As<Rendering::Shader>();

		// TODO: Remove this texture binding method
		shader->bind();
		if (m_texture != NULL)
		{
			m_texture.As<Rendering::Texture>()->bind(0);
		}

		{
			int texIndex = 0;
			for (auto const& x : m_textures)
			{
				shader->setTexture(x.first, texIndex++, x.second.As<Rendering::Texture>());
			}
		}

		for (auto const& x : m_floatValues)
		{
			shader->setFloat(x.first, x.second);
		}

		for (auto const& x : m_floatArrayValues)
		{
			shader->setFloatArray(x.first, x.second);
		}

		for (auto const& x : m_intValues)
		{
			shader->setInt(x.first, x.second);
		}

		for (auto const& x : m_vec3Values)
		{
			shader->setVec3(x.first, x.second);
		}

		for (auto const& x : m_vec4Values)
		{
			shader->setVec4(x.first, x.second);
		}

		for (auto const& x : m_mat3Values)
		{
			shader->setMat3(x.first, x.second);
		}

		for (auto const& x : m_mat4Values)
		{
			shader->setMat4(x.first, x.second);
		}

		for (auto const& x : m_mat4ArrayValues)
		{
			shader->setMat4Array(x.first, x.second);
		}

	}

	void Material::copyFrom(std::shared_ptr<Material> other)
	{
		m_boolValues = other->m_boolValues;
		m_floatValues = other->m_floatValues;
		m_floatArrayValues = other->m_floatArrayValues;
		m_intValues = other->m_intValues;
		m_mat3Values = other->m_mat3Values;
		m_mat4Values = other->m_mat4Values;
		m_mat4ArrayValues = other->m_mat4ArrayValues;
		m_textures = other->m_textures;
		m_texture = other->m_texture;
		m_vec3Values = other->m_vec3Values;
		m_vec4Values = other->m_vec4Values;
		m_shader = other->m_shader;
	}

	void Material::unbind()
	{
		assert(m_shader);

		auto shader = m_shader.As<Rendering::Shader>();

		{
			int texIndex = 0;
			for (auto const& x : m_textures)
			{
				shader->clearTexture(x.first, texIndex++, x.second.As<Rendering::Texture>()->getTextureType());
			}
		}

		for (auto const& x : m_floatValues)
		{
			shader->clearFloat(x.first);
		}

		for (auto const& x : m_intValues)
		{
			shader->clearInt(x.first);
		}

		for (auto const& x : m_vec3Values)
		{
			shader->clearVec3(x.first);
		}

		for (auto const& x : m_vec4Values)
		{
			shader->clearVec4(x.first);
		}

		for (auto const& x : m_mat3Values)
		{
			shader->clearMat3(x.first);
		}

		for (auto const& x : m_mat4Values)
		{
			shader->clearMat4(x.first);
		}
	}

	void Material::setBool(const std::string& name, bool const& value)
	{
		m_boolValues[name] = value;
	}

	void Material::setColorRGB(const std::string& name, Color const& value)
	{
		m_vec3Values[name] = glm::vec3(value.r, value.g, value.b);
	}

	void Material::setColorRGBA(const std::string& name, Color const& value)
	{
		m_vec4Values[name] = glm::vec4(value.r, value.g, value.b, value.a);
	}

	void Material::setFloat(const std::string& name, float const& value)
	{
		m_floatValues[name] = value;
	}

	void Material::setFloatArray(const std::string & name, std::vector<float> value)
	{
		m_floatArrayValues[name] = value;
	}

	void Material::setInt(const std::string& name, int const& value)
	{
		m_intValues[name] = value;
	}

	void Material::setMat3(const std::string& name, glm::mat3 const& value)
	{
		m_mat3Values[name] = value;
	}

	void Material::setMat4(const std::string& name, glm::mat4 const& value)
	{
		m_mat4Values[name] = value;
	}

	void Material::setMat4Array(const std::string& name, std::vector<glm::mat4> matrixArray)
	{
		m_mat4ArrayValues[name] = matrixArray;
	}

	void Material::setTexture(const std::string& name, std::shared_ptr<Rendering::Texture> texture)
	{
		auto res = std::make_shared<Resources::TextureResource>();
		res->setData(texture);
		m_textures[name] = res;
	}

	void Material::setTexture(const std::string& name, ResourceRef<Resources::TextureResource> texture)
	{
		m_textures[name] = texture;
	}

	void Material::setTextureSlot(int const& slot, ResourceRef<Resources::TextureResource> texture)
	{
		m_texture = texture;
	}

	void Material::setVec3(const std::string& name, glm::vec3 const& value)
	{
		m_vec3Values[name] = value;
	}

	void Material::setVec4(const std::string& name, glm::vec4 const& value)
	{
		m_vec4Values[name] = value;
	}

	bool Material::boolExists(const std::string& name)
	{
		return m_boolValues.find(name) != m_boolValues.end();
	}

	bool Material::colorRGBExists(const std::string& name)
	{
		return m_vec3Values.find(name) != m_vec3Values.end();
	}

	bool Material::colorRGBAExists(const std::string& name)
	{
		return m_vec4Values.find(name) != m_vec4Values.end();
	}

	bool Material::floatExists(const std::string& name)
	{
		return m_floatValues.find(name) != m_floatValues.end();
	}

	bool Material::floatArrayExists(const std::string & name)
	{
		return m_floatArrayValues.find(name) != m_floatArrayValues.end();
	}

	bool Material::intExists(const std::string& name)
	{
		return m_intValues.find(name) != m_intValues.end();
	}

	bool Material::mat3Exists(const std::string& name)
	{
		return m_mat3Values.find(name) != m_mat3Values.end();
	}

	bool Material::mat4Exists(const std::string& name)
	{
		return m_mat4Values.find(name) != m_mat4Values.end();
	}

	bool Material::mat4ArrayExists(const std::string& name)
	{
		return m_mat4ArrayValues.find(name) != m_mat4ArrayValues.end();
	}

	bool Material::textureExists(const std::string& name)
	{
		return m_textures.find(name) != m_textures.end();
	}

	bool Material::textureSlotExists(int slot)
	{
		return m_texture.As<Rendering::Texture>() != nullptr;
	}

	bool Material::vec3Exists(const std::string& name)
	{
		return m_vec3Values.find(name) != m_vec3Values.end();
	}

	bool Material::vec4Exists(const std::string& name)
	{
		return m_vec4Values.find(name) != m_vec4Values.end();
	}

	bool Material::getBool(const std::string& name)
	{
		auto it = m_boolValues.find(name);
		if (it == m_boolValues.end())
		{
			return false;
		}
		else
		{
			return (*it).second;
		}
	}

	Color Material::getColorRGB(const std::string& name)
	{
		auto it = m_vec3Values.find(name);
		if (it == m_vec3Values.end())
		{
			return Color();
		}
		else
		{
			return (*it).second;
		}
	}

	Color Material::getColorRGBA(const std::string& name)
	{
		auto it = m_vec4Values.find(name);
		if (it == m_vec4Values.end())
		{
			return Color();
		}
		else
		{
			return (*it).second;
		}
	}

	float Material::getFloat(const std::string& name)
	{
		auto it = m_floatValues.find(name);
		if (it == m_floatValues.end())
		{
			return 0.0f;
		}
		else
		{
			return (*it).second;
		}
	}

	std::vector<float> Material::getFloatArray(const std::string & name)
	{
		auto it = m_floatArrayValues.find(name);
		if (it == m_floatArrayValues.end())
		{
			return std::vector<float>();
		}
		else
		{
			return (*it).second;
		}
	}

	int Material::getInt(const std::string& name)
	{
		auto it = m_intValues.find(name);
		if (it == m_intValues.end())
		{
			return 0;
		}
		else
		{
			return (*it).second;
		}
	}

	glm::mat3 Material::getMat3(const std::string& name)
	{
		auto it = m_mat3Values.find(name);
		if (it == m_mat3Values.end())
		{
			return glm::mat3();
		}
		else
		{
			return (*it).second;
		}
	}

	glm::mat4 Material::getMat4(const std::string& name)
	{
		auto it = m_mat4Values.find(name);
		if (it == m_mat4Values.end())
		{
			return glm::mat4();
		}
		else
		{
			return (*it).second;
		}
	}

	std::vector<glm::mat4> Material::getMat4Array(const std::string& name)
	{
		auto it = m_mat4ArrayValues.find(name);
		if (it == m_mat4ArrayValues.end())
		{
			return std::vector<glm::mat4>();
		}
		else
		{
			return (*it).second;
		}
	}

	ResourceRef<Resources::TextureResource> Material::getTexture(const std::string& name)
	{
		auto it = m_textures.find(name);
		if (it == m_textures.end())
		{
			return nullptr;
		}
		else
		{
			return (*it).second;
		}
	}

	ResourceRef<Resources::TextureResource> Material::getTextureSlot(int slot)
	{
		return m_texture;
	}

	glm::vec3 Material::getVec3(const std::string& name)
	{
		auto it = m_vec3Values.find(name);
		if (it == m_vec3Values.end())
		{
			return glm::vec3();
		}
		else
		{
			return (*it).second;
		}
	}

	glm::vec4 Material::getVec4(const std::string& name)
	{
		auto it = m_vec4Values.find(name);
		if (it == m_vec4Values.end())
		{
			return glm::vec4();
		}
		else
		{
			return (*it).second;
		}
	}

    AssetData::MaterialData Material::ToData() const
    {
		auto data = AssetData::MaterialData();

		if (m_shader && m_shader.As<Shader>()->isValid())
		{
			data.ShaderId = m_shader->getId();
		}

		for (auto texture : m_textures)
		{
			data.Textures.push_back(AssetData::MaterialPropertyData<ResourceRef<Resources::TextureResource>>(texture.first, texture.second));
		}

		for (auto prop : m_boolValues) data.Bools.push_back(AssetData::MaterialPropertyData<bool>(prop.first, prop.second));
		for (auto prop : m_intValues) data.Ints.push_back(AssetData::MaterialPropertyData<int>(prop.first, prop.second));
		for (auto prop : m_floatValues) data.Floats.push_back(AssetData::MaterialPropertyData<float>(prop.first, prop.second));
		for (auto prop : m_floatArrayValues) data.FloatArrays.push_back(AssetData::MaterialPropertyData<std::vector<float>>(prop.first, prop.second));
		for (auto prop : m_vec3Values) data.Float3s.push_back(AssetData::MaterialPropertyData<glm::vec3>(prop.first, prop.second));
		for (auto prop : m_vec4Values) data.Float4s.push_back(AssetData::MaterialPropertyData<glm::vec4>(prop.first, prop.second));
		for (auto prop : m_mat3Values) data.Float3x3s.push_back(AssetData::MaterialPropertyData<glm::mat3>(prop.first, prop.second));
		for (auto prop : m_mat4Values) data.Float4x4s.push_back(AssetData::MaterialPropertyData<glm::mat4>(prop.first, prop.second));
		for (auto prop : m_mat4ArrayValues) data.Float4x4Arrays.push_back(AssetData::MaterialPropertyData<std::vector<glm::mat4>>(prop.first, prop.second));

		return data;
    }

}
