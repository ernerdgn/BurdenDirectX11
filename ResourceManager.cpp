#include "ResourceManager.h"
#include <filesystem>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourcePtr ResourceManager::createResourceFromFile(const wchar_t* file_path)
{
	std::wstring full_path = std::filesystem::absolute(file_path);

	auto iterator = m_map_of_resources.find(full_path);
	if (iterator != m_map_of_resources.end()) return iterator->second;

	Resource* raw_resource = this->createResourceFromFileConcrete(full_path.c_str());
	if (raw_resource)
	{
		ResourcePtr resource(raw_resource);
		m_map_of_resources[full_path] = resource;
		return resource;
	}

	return nullptr;
}
