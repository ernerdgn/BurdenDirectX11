#pragma once
#include <memory>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;
class RenderSystem;
class GraphicsEngine;
class ResourceManager;
class Resource;
class TextureManager;
class Texture;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;