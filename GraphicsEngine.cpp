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
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
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
