#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

#include <iostream>

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
}

bool ConstantBuffer::load(void* buffer, UINT size_buffer)
{
	if (m_buffer)
	{
		std::cout << "released m_buffer!!!!!" << std::endl;
		m_buffer->Release();
	}

	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = size_buffer;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = buffer;

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer))) return false;

	return true;
}

//void ConstantBuffer::update(DeviceContext* dev_context, void* buffer)
//{
//	dev_context->m_device_context->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, NULL, NULL);
//}

void ConstantBuffer::update(DeviceContext* dev_context, void* buffer)
{
	dev_context->m_device_context->UpdateSubresource(m_buffer, NULL, NULL, buffer, NULL, NULL);

	//This line is here for the "get set" error
	//if (dev_context && dev_context->m_device_context && m_buffer)
	//{
	//	dev_context->m_device_context->UpdateSubresource(m_buffer, NULL, NULL, buffer, NULL, NULL);
	//}
	//else
	//{
	//	if (!dev_context) std::cout << "dev_context err!" << std::endl;
	//	if (!dev_context->m_device_context) std::cout << "dev_context->m_device_context err!" << std::endl;
	//	if (!m_buffer) std::cout << "m_buffer err!" << std::endl;  //error is the m_buffer
	//}
}

bool ConstantBuffer::release()
{
	if (m_buffer) m_buffer->Release();
	delete this;
	return true;
}
