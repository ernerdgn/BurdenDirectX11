#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class PixelShader
{
	PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);
	~PixelShader();

private:
	ID3D11PixelShader * m_pixel_shader;

	RenderSystem* m_system = nullptr;

	friend class RenderSystem;
	friend class DeviceContext;
	friend class AppWindow;
};

