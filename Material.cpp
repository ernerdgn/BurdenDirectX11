#include "Material.h"

Material::Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	/* VERTEX SHADER */
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(vertex_shader_path, "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if (!m_vertex_shader) throw std::exception("GraphicsEngine Error(1) : Material (VertexShader)");

	/* PIXEL SHADER */
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(pixel_shader_path, "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if (!m_pixel_shader) throw std::exception("GraphicsEngine Error(1) : Material (PixelShader)");
}

Material::Material(const MaterialPtr& material)
{
	m_vertex_shader = material->m_vertex_shader;
	m_pixel_shader = material->m_pixel_shader;
}

Material::~Material()
{
}

void Material::addTexture(const TexturePtr& texture)
{
	m_texture_vector.push_back(texture);
}

void Material::removeTexture(unsigned int index)
{
	if (index >= this->m_texture_vector.size()) return;
	m_texture_vector.erase(m_texture_vector.begin() + index);
}

void Material::setData(void* data, unsigned int size)
{
	if (!m_constant_buffer) m_constant_buffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(data, size);  //constant buffer loading
	else m_constant_buffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data);
}

void Material::setCullMode(CULL_MODE cull_mode)
{
	m_cull_mode = cull_mode;
}

CULL_MODE Material::getCullMode()
{
	return m_cull_mode;
}
