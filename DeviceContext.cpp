#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"

#include <iostream>
#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context,RenderSystem* system) : m_device_context(device_context), m_system(system)
{
}

DeviceContext::~DeviceContext()
{
	m_device_context->Release();
}

void DeviceContext::clearRenderTargetColor(SwapChain* swap_chain, float r, float g, float b, float a)
{
	FLOAT clear_color[] = { r, g, b, a };
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, NULL);
}

void DeviceContext::setVertexBuffer(VertexBuffer* vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;

	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);

	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::setIndexBuffer(IndexBuffer* index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* constant_buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &constant_buffer->m_buffer);  //NOT GET! SET...
}

void DeviceContext::setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* constant_buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &constant_buffer->m_buffer);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT base_vertex_location, UINT start_index_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count, start_index_location, base_vertex_location);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = .0f;
	viewport.MaxDepth = 1.0f;

	m_device_context->RSSetViewports(1, &viewport);
}

void DeviceContext::setVertexShader(VertexShader* vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->m_vertex_shader, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShader* pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->m_pixel_shader, nullptr, 0);
}