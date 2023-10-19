#pragma once
#include <memory>
#include "Vector3D.h"

class Matrix4x4
{
public:
	Matrix4x4()
	{

	}

	void setIdentityMatrix()
	{
		::memset(m_matrix, 0, sizeof(float) * 16);  //fill with 0s
		//for (int i = 0; i <= 3; i++) m_matrix[i][i] = 1;
		m_matrix[0][0] = 1;
		m_matrix[1][1] = 1;
		m_matrix[2][2] = 1;
		m_matrix[3][3] = 1;
	}

	void setTranslationMatrix(const Vector3D& translation)
	{
		setIdentityMatrix();
		m_matrix[3][0] = translation.m_x;
		m_matrix[3][1] = translation.m_y;
		m_matrix[3][2] = translation.m_z;
	}

	void setScaleMatrix(const Vector3D& scale)
	{
		setIdentityMatrix();
		m_matrix[0][0] = scale.m_x;
		m_matrix[1][1] = scale.m_y;
		m_matrix[2][2] = scale.m_z;
	}

	void setRotationXMatrix(float x)
	{
		m_matrix[1][1] = cos(x);
		m_matrix[1][2] = sin(x);
		m_matrix[2][1] = -sin(x);
		m_matrix[2][2] = cos(x);
	}

	void setRotationYMatrix(float x)
	{
		m_matrix[0][0] = cos(x);
		m_matrix[0][2] = -sin(x);
		m_matrix[2][0] = sin(x);
		m_matrix[2][2] = cos(x);
	}

	void setRotationZMatrix(float x)
	{
		m_matrix[0][0] = cos(x);
		m_matrix[0][1] = sin(x);
		m_matrix[1][0] = -sin(x);
		m_matrix[1][1] = cos(x);
	}

	void operator *=(Matrix4x4& matrix)
	{
		Matrix4x4 result;
		for (int i = 0; i <= 3; i++)
		{
			for (int j = 0; j <= 3; j++)
			{
				result.m_matrix[i][j] = 
					m_matrix[i][0] * matrix.m_matrix[0][j] + m_matrix[i][1] * matrix.m_matrix[1][j] +
					m_matrix[i][2] * matrix.m_matrix[2][j] + m_matrix[i][3] * matrix.m_matrix[3][j];
			}
		}
		
		::memcpy(m_matrix, result.m_matrix, sizeof(float) * 16);
	}

	void setOrthogonalProjectionMatrix(float width, float height, float near_plane, float far_plane)
	{
		setIdentityMatrix();
		m_matrix[0][0] = 2.0f / width;
		m_matrix[1][1] = 2.0f / height;
		m_matrix[2][2] = 1.0f / (far_plane - near_plane);
		m_matrix[3][2] = -(near_plane / (far_plane - near_plane));
	}

	~Matrix4x4()
	{

	}

	float m_matrix[4][4] = {};
};