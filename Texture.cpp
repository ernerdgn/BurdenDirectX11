#include "Texture.h"
#include "GraphicsEngine.h"

Texture::Texture(const wchar_t* full_path) : Resource(full_path)
{
	DirectX::ScratchImage img_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_NONE, nullptr, img_data);

	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_d3d_device,
			img_data.GetImages(), img_data.GetImageCount(), img_data.GetMetadata(), &m_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = img_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = img_data.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texture,
			&desc, &m_shader_resource_view);
	}
	else { throw std::exception("ResourceManager Error (5): Texture"); }
}

Texture::~Texture()
{
	m_shader_resource_view->Release();
	m_texture->Release();
}
