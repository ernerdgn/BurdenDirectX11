#pragma once
#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();
	bool load(void* buffer, UINT size_buffer);
	void update(DeviceContext* dev_context, void* buffer);
	bool release();

private:
	ID3D11Buffer* m_buffer;

	friend class DeviceContext;
};