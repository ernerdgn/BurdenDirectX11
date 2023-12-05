#pragma once
#include <d3d11.h>  //directX11
#include <d3dcompiler.h>  //shader compilation
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
	GraphicsEngine();
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem();

	static GraphicsEngine* get();
	static void create();
	static void release();

private:
	RenderSystem* m_render_system = nullptr;
	static GraphicsEngine* m_engine;
};


/*
error code map

(1) - graphicsEngine (component) creation error
(2) - already exists
(3) - window registeration
(4) - window (component) creation error





*/