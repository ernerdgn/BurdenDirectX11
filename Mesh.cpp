#include "Mesh.h"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>

//#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <codecvt>

#include "GraphicsEngine.h"
#include "VertexMesh.h"

Mesh::Mesh(const wchar_t* file_path) : Resource(file_path)
{
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string error;

	int size_needed = WideCharToMultiByte(CP_OEMCP, 0, &file_path[0], wcslen(file_path), NULL, 0, NULL, NULL);
	std::string input_file(size_needed, 0);
	WideCharToMultiByte(CP_OEMCP, 0, &file_path[0], -1, &input_file[0], size_needed, NULL, NULL);
	//std::string input_file = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(file_path);

	bool check = tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &error, input_file.c_str());

	if (!error.empty())
	{
		std::cout << "MESH ERROR: " << error << std::endl;
		throw std::exception("ResourceManager Error (7): Mesh");
	}
	if (!check) throw std::exception("ResourceManager Error (7): Mesh");

	// this check is for loading only one obj, DELETE WHEN THE TIME COMES ki belaný sikmiim
	//if (shapes.size() > 1) throw std::exception("ResourceManager Error (7): Mesh");

	std::vector<VertexMesh> vertices_list;
	std::vector<unsigned int> indices_list;

	for (size_t shape = 0; shape < shapes.size(); shape++)
	{
		size_t index_offset = 0;
		vertices_list.reserve(shapes[shape].mesh.indices.size());
		indices_list.reserve(shapes[shape].mesh.indices.size());

		for (size_t face = 0; face < shapes[shape].mesh.num_face_vertices.size(); face++)
		{
			unsigned char num_face_vertices = shapes[shape].mesh.num_face_vertices[face];

			for (unsigned char vertex = 0; vertex < num_face_vertices; vertex++)
			{
				tinyobj::index_t index = shapes[shape].mesh.indices[index_offset + vertex];

				// vertex position data
				tinyobj::real_t vX = attributes.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vY = attributes.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vZ = attributes.vertices[index.vertex_index * 3 + 2];

				// vertex texture coordinate data
				tinyobj::real_t tX = attributes.texcoords[index.texcoord_index * 2 + 0];
				tinyobj::real_t tY = attributes.texcoords[index.texcoord_index * 2 + 1];

				VertexMesh vertex_m(Vector3D(vX, vY, vZ), Vector2D(tX, tY));
				vertices_list.push_back(vertex_m);

				indices_list.push_back(index_offset + vertex);
			}

			index_offset += num_face_vertices;
		}
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVMLOUTShaderByteCodeSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&vertices_list[0], sizeof(VertexMesh),
		(UINT)vertices_list.size(), shader_byte_code, (UINT)size_shader);

	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&indices_list[0], (UINT)indices_list.size());
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}
