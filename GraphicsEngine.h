#pragma once
#include <d3d11.h>  //directX11
#include <d3dcompiler.h>  //shader compilation
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool init();
	bool release();

	RenderSystem* getRenderSystem();

	static GraphicsEngine* get();

private:
	RenderSystem* m_render_system = nullptr;
};


/*
error code map
GraphicsEngineErrors
(1) - creation error





*/