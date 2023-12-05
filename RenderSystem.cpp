#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"

#include <d3dcompiler.h>
#include <iostream>
#include <exception>

RenderSystem::RenderSystem()
{	
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;

	//ID3D11DeviceContext* m_imm_context;  //-//
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index],
			NULL, NULL,
			feature_levels, num_feature_levels,
			D3D11_SDK_VERSION,
			&m_d3d_device, &m_feature_level, &m_imm_context);

		if (SUCCEEDED(res))
		{
			break;
			++driver_type_index;
		}
	}

	if (FAILED(res)) throw std::exception("GraphicsEngine Error (1): RenderSystem");

	m_imm_device_context = std::make_shared<DeviceContext>(m_imm_context, this);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);
}

RenderSystem::~RenderSystem()
{
	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	m_d3d_device->Release();
}

void RenderSystem::releaseCompiledShader()
{
	if (m_blob) m_blob->Release();
}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChainPtr swap_chain = nullptr;

	try
	{
		swap_chain = std::make_shared<SwapChain>(hwnd, width, height, this);
	}

	catch(...) {}

	return swap_chain;
}

DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	VertexBufferPtr vertex_buffer = nullptr;

	try
	{
		vertex_buffer = std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
	}

	catch (...) {}

	return vertex_buffer;

}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBufferPtr constant_buffer = nullptr;

	try
	{
		constant_buffer = std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
	}

	catch (...) {}
	return constant_buffer;
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBufferPtr index_buffer = nullptr;

	try
	{
		index_buffer = std::make_shared<IndexBuffer>(list_indices, size_list, this);
	}

	catch (...) {}

	return index_buffer;
}

VertexShaderPtr RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShaderPtr vertex_shader = nullptr;

	try
	{
		vertex_shader = std::make_shared<VertexShader>(shader_byte_code, byte_code_size, this);
	}

	catch (...) {}

	return vertex_shader;
}

PixelShaderPtr RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShaderPtr pixel_shader = nullptr;

	try
	{
		pixel_shader = std::make_shared<PixelShader>(shader_byte_code, byte_code_size, this);
	}

	catch (...) {}

	return pixel_shader;
}

bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* shader_byte_code_size)
{
	ID3DBlob* err_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &err_blob)))
	{
		if (err_blob) err_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*shader_byte_code_size = m_blob->GetBufferSize();

	return true;
}

bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* shader_byte_code_size)
{
	ID3DBlob* err_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &err_blob)))
	{
		if (err_blob)
		{
			std::cout << "!!! WARNING: " << (char*)err_blob->GetBufferPointer() << std::endl;
			err_blob->Release();
		}
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*shader_byte_code_size = m_blob->GetBufferSize();

	return true;
}