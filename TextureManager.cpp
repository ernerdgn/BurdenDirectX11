#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::createTextureFromFile(const wchar_t* file_path)
{
	return std::static_pointer_cast<Texture>(createResourceFromFile(file_path));
}

Resource* TextureManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	Texture* texture = nullptr;
	try
	{
		texture = new Texture(file_path);
	}
	catch (...) { throw std::exception("ResourceManager Error (5): TextureManager (Texture)"); }

	return texture;
}
