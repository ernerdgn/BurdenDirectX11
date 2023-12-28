#include "GraphicsEngine.h"
#include <exception>

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{	
	try
	{
		m_render_system = new RenderSystem();
	}
	catch (...) { throw std::exception("GraphicsEngine Error (1): GraphicsEngine (RenderSystem)"); }

	try
	{
		m_texture_manager = new TextureManager();
	}
	catch (...) { throw std::exception("GraphicsEngine Error (1): GraphicsEngine (TextureManager)"); }

	try
	{
		m_mesh_manager = new MeshManager();
	}
	catch (...) { throw std::exception("GraphicsEngine Error (1): GraphicsEngine (MeshManager)"); }

	// hlsl for obj
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	m_render_system->compileVertexShader(L"VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	::memcpy(m_mesh_layout_byte_code, shader_byte_code, size_shader);
	m_mesh_layout_size = size_shader;
	m_render_system->releaseCompiledShader();
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete m_mesh_manager;
	delete m_texture_manager;
	delete m_render_system;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_texture_manager;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return m_mesh_manager;
}

MaterialPtr GraphicsEngine::createMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	MaterialPtr material = nullptr;

	try
	{
		material = std::make_shared<Material>(vertex_shader_path, pixel_shader_path);
	}

	catch (...) {}

	return material;
}

MaterialPtr GraphicsEngine::createMaterial(const MaterialPtr& material)
{
	MaterialPtr material_ = nullptr;

	try
	{
		material_ = std::make_shared<Material>(material);
	}

	catch (...) {}

	return material_;
}

void GraphicsEngine::setMaterial(const MaterialPtr& material)
{
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState((material->m_cull_mode == CULL_MODE_FRONT));
	//set constant buffer
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(material->m_vertex_shader, material->m_constant_buffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(material->m_pixel_shader, material->m_constant_buffer);

	//implement the prepared shaders to graphic pipeline to be able to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(material->m_vertex_shader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(material->m_pixel_shader);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(material->m_pixel_shader, &material->m_texture_vector[0], material->m_texture_vector.size());
}

void GraphicsEngine::getVMLOUTShaderByteCodeSize(void** byte_code, size_t* size)
{
	*byte_code = m_mesh_layout_byte_code;
	*size = m_mesh_layout_size;
}


GraphicsEngine* GraphicsEngine::get()
{
	return m_engine;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine) throw std::exception("GraphicsEngine Error (2): GraphicsEngine");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine) return;
	delete GraphicsEngine::m_engine;
}
