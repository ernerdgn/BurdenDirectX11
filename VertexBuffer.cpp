#include "VertexBuffer.h"
#include "RenderSystem.h"
#include <exception>

VertexBuffer::VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader, RenderSystem* system) : m_layout(0), m_buffer(0), m_system(system)
{
	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = size_list * size_vertex;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer))) throw std::exception("GraphicsEngine Error (1): VertexBuffer");

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{
			"POSITION",  //semantic name
			0,  //semantic index
			DXGI_FORMAT_R32G32B32_FLOAT,  //format
			0,  //input slot
			0,  //aligned byte offset
			D3D11_INPUT_PER_VERTEX_DATA,  //input slot class
			0  //instance data step rate
		},

		//{
		//	"POSITION",  //semantic name
		//	1,  //semantic index
		//	DXGI_FORMAT_R32G32B32_FLOAT,  //format
		//	0,  //input slot
		//	12,  //aligned byte offset
		//	D3D11_INPUT_PER_VERTEX_DATA,  //input slot class
		//	0  //instance data step rate
		//},

		{
			"COLOR",  //semantic name
			0,  //semantic index
			DXGI_FORMAT_R32G32B32_FLOAT,  //format
			0,  //input slot
			12,  //aligned byte offset
			D3D11_INPUT_PER_VERTEX_DATA,  //input slot class
			0  //instance data step rate
		},

		{
			"COLOR",  //semantic name
			1,  //semantic index
			DXGI_FORMAT_R32G32B32_FLOAT,  //format
			0,  //input slot
			24,  //aligned byte offset
			D3D11_INPUT_PER_VERTEX_DATA,  //input slot class
			0  //instance data step rate
		}
	};

	UINT size_layout = ARRAYSIZE(layout);

	if (FAILED(m_system->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout))) throw std::exception("GraphicsEngine Error (1): InputLayout");
}

VertexBuffer::~VertexBuffer()
{
	m_layout->Release();
	m_buffer->Release();
}

UINT VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}