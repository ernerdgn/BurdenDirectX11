#pragma once
#include <d3d11.h>  //directX11
#include <d3dcompiler.h>  //shader compilation
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Material.h"
#include "DeviceContext.h"

class GraphicsEngine
{
	GraphicsEngine();
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();
	MeshManager* getMeshManager();

	MaterialPtr createMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	MaterialPtr createMaterial(const MaterialPtr& material);
	void setMaterial(const MaterialPtr& material);

	void getVMLOUTShaderByteCodeSize(void** byte_code, size_t* size);

	static GraphicsEngine* get();
	static void create();
	static void release();

private:
	RenderSystem* m_render_system = nullptr;
	TextureManager* m_texture_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;
	static GraphicsEngine* m_engine;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;

};


/*
error code map

(1) - graphicsEngine (component) creation error
(2) - already exists
(3) - window registeration
(4) - window (component) creation error
(5) - resourceManager (component) creation error
(6) - textureManager creating texture from file failed
(7) - obj load is failed / not loaded succesfully




*/