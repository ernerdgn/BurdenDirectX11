#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;
class AppWindow;

class PixelShader
{
	PixelShader();
	~PixelShader();
	void release();

private:
	bool init(const void* shader_byte_code, size_t byte_code_size);
	ID3D11PixelShader * m_pixel_shader;

	friend class GraphicsEngine;
	friend class DeviceContext;
	friend class AppWindow;
};

