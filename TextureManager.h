#pragma once
#include "ResourceManager.h"
#include <exception>

class TextureManager : public ResourceManager
{
public:
	TextureManager();
	~TextureManager();
	TexturePtr createTextureFromFile(const wchar_t* file_path);

	//inherited via ResourceManager
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path);
};

