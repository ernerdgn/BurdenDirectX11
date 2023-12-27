#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();
	bool present(bool vsync);
	void resize(unsigned int width, unsigned int height);
	void setFullscreen(bool fullscreen_flag, unsigned int width, unsigned int height);

private:
	void reloadBuffers(unsigned int width, unsigned int height);

	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_rtv = nullptr;
	ID3D11DepthStencilView* m_dsv = nullptr;

	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
};

