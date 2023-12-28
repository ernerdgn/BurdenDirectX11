#pragma once
#include "Resource.h"
#include <DirectXTex.h> //directX texture lib
#include <exception>
#include <d3d11.h>

class Texture : public Resource
{
public:
	Texture(const wchar_t* full_path);
	~Texture();

private:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_resource_view = nullptr;
	ID3D11SamplerState* m_sampler_state = nullptr;
	
	friend class DeviceContext;
};

