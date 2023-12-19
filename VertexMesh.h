#pragma once
#include "Vector3D.h"  //to store vertex position
#include "Vector2D.h"  //to store vertex texcoord

class VertexMesh
{
public:
	VertexMesh() : m_position(), m_texcoord(), m_normals()
	{

	}
	VertexMesh(Vector3D position, Vector2D texcoord, Vector3D normals) : m_position(position), m_texcoord(texcoord), m_normals(normals)
	{

	}
	VertexMesh(const VertexMesh& vertex_mesh) : m_position(vertex_mesh.m_position), m_texcoord(vertex_mesh.m_texcoord), m_normals(vertex_mesh.m_normals)
	{

	}

	~VertexMesh()
	{

	}

private:
	Vector3D m_position;
	Vector2D m_texcoord;
	Vector3D m_normals;
};