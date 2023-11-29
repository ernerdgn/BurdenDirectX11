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

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

bool RenderSystem::init()
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

	if (FAILED(res)) return false;

	m_imm_device_context = new DeviceContext(m_imm_context, this);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	return true;
}

bool RenderSystem::release()
{
	if (m_vs)m_vs->Release();
	if (m_ps)m_ps->Release();

	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	delete m_imm_device_context;
	m_d3d_device->Release();
	return true;
}

void RenderSystem::releaseCompiledShader()
{
	if (m_blob) m_blob->Release();
}

SwapChain* RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChain* swap_chain = nullptr;

	try
	{
		swap_chain = new SwapChain(hwnd, width, height, this);
	}

	catch(...) {}

	return swap_chain;
}

DeviceContext* RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	VertexBuffer* vertex_buffer = nullptr;

	try
	{
		vertex_buffer = new VertexBuffer(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
	}

	catch (...) {}

	return vertex_buffer;

}

ConstantBuffer* RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBuffer* constant_buffer = nullptr;

	try
	{
		constant_buffer = new ConstantBuffer(buffer, size_buffer, this);
	}

	catch (...) {}
	return constant_buffer;
}

IndexBuffer* RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBuffer* index_buffer = nullptr;

	try
	{
		index_buffer = new IndexBuffer(list_indices, size_list, this);
	}

	catch (...) {}

	return index_buffer;
}

VertexShader* RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vertex_shader = nullptr;

	try
	{
		vertex_shader = new VertexShader(shader_byte_code, byte_code_size, this);
	}

	catch (...) {}

	return vertex_shader;
}

PixelShader* RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* pixel_shader = nullptr;

	try
	{
		pixel_shader = new PixelShader(shader_byte_code, byte_code_size, this);
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