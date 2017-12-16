#pragma once

class Shader;
class Texture;
class RenderTexture;

class Material
{
public:
	Material();
	~Material();

	inline void setShader(Shader* shader) { m_shader = shader; }
	inline Shader* getShader() const { return m_shader; }
	void setTextureSlot(int slot, Texture* texture);
	void bind();
private:
	Shader* m_shader;
	// TODO: Replace this with a BST for multiple textures
	Texture* m_texture;
};

