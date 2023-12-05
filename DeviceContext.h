#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);
	~DeviceContext();

	void clearRenderTargetColor(SwapChainPtr swap_chain, float r, float g, float b, float a);
	void setVertexBuffer(VertexBufferPtr vertex_buffer);
	void setIndexBuffer(IndexBufferPtr index_buffer);

	void setConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr constant_buffer);
	void setConstantBuffer(PixelShaderPtr pixel_shader, ConstantBufferPtr constant_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT base_vertex_location, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);
	
	void setViewportSize(UINT width, UINT height);
	void setVertexShader(VertexShaderPtr vertex_shader);
	void setPixelShader(PixelShaderPtr pixel_shader);

private:
	ID3D11DeviceContext* m_device_context;

	RenderSystem* m_system = nullptr;

	friend class ConstantBuffer;
};

