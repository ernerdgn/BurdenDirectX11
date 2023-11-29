#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexShader
{
	VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
	~VertexShader();

private:
	ID3D11VertexShader* m_vertex_shader;

	RenderSystem* m_system = nullptr;

	friend class RenderSystem;
	friend class DeviceContext;
	friend class AppWindow;
};

