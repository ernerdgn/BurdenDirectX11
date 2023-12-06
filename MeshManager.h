#pragma once
#include "ResourceManager.h"
#include <exception>

class MeshManager : public ResourceManager
{
public:
	MeshManager();
	~MeshManager();
	MeshPtr createMeshFromFile(const wchar_t* file_path);

	//inherited via ResourceManager
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path);
};
