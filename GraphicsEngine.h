#pragma once
#include <d3d11.h>  //directX11
#include <d3dcompiler.h>  //shader compilation
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"

class GraphicsEngine
{
	GraphicsEngine();
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();

	static GraphicsEngine* get();
	static void create();
	static void release();

private:
	RenderSystem* m_render_system = nullptr;
	TextureManager* m_texture_manager = nullptr;
	static GraphicsEngine* m_engine;
};


/*
error code map

(1) - graphicsEngine (component) creation error
(2) - already exists
(3) - window registeration
(4) - window (component) creation error
(5) - resourceManager (component) creation error
(6) - textureManager creating texture from file failed




*/