#include "SwapChain.h"
#include "RenderSystem.h"
#include <exception>

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system) : m_system(system)
{
	//CreateSwapChain() arguments
	//device
	ID3D11Device* device = m_system->m_d3d_device;

	//swapchain description
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = TRUE;

	HRESULT hr = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if (FAILED(hr)) throw std::exception("GraphicsEngine Error (1): SwapChain");

	reloadBuffers(width, height);
}

SwapChain::~SwapChain()
{
	m_rtv->Release();
	m_swap_chain->Release();
}

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
	return true;
}

void SwapChain::resize(unsigned int width, unsigned int height)
{
	//clear render target view and depth stencil view before process
	if (m_rtv) m_rtv->Release();
	if (m_dsv) m_dsv->Release();

	m_swap_chain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	reloadBuffers(width, height);
}

void SwapChain::setFullscreen(bool fullscreen_flag, unsigned int width, unsigned int height)
{
	resize(width, height);
	m_swap_chain->SetFullscreenState(fullscreen_flag, nullptr);
}

void SwapChain::reloadBuffers(unsigned int width, unsigned int height)
{
	//device
	ID3D11Device* device = m_system->m_d3d_device;

	//get back buffer color
	//create rtv
	ID3D11Texture2D* buffer = NULL;
	HRESULT hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr)) throw std::exception("GraphicsEngine Error (1): SwapChain");

	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(hr)) throw std::exception("GraphicsEngine Error (1): SwapChain");

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture_desc.Width = width;
	texture_desc.Height = height;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture_desc.MipLevels = 1;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.MiscFlags = 0;
	texture_desc.ArraySize = 1;
	texture_desc.CPUAccessFlags = 0;

	hr = device->CreateTexture2D(&texture_desc, nullptr, &buffer);

	if (FAILED(hr)) throw std::exception("GraphicsEngine Error (1): SwapChain");

	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsv);
	buffer->Release();

	if (FAILED(hr)) throw std::exception("GraphicsEngine Error (1): SwapChain");
}
