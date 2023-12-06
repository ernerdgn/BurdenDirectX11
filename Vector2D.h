#pragma once
#pragma once

class Vector2D
{
public:
	Vector2D() : m_x(0), m_y(0)
	{

	}
	Vector2D(float x, float y) : m_x(x), m_y(y)
	{

	}
	Vector2D(const Vector2D& vector) : m_x(vector.m_x), m_y(vector.m_y)
	{

	}

	Vector2D operator *(float num)
	{
		return Vector2D(m_x * num, m_y * num);
	}

	Vector2D operator +(Vector2D adding_vec)
	{
		return Vector2D(m_x + adding_vec.m_x, m_y + adding_vec.m_y);
	}

	~Vector2D()
	{

	}

	float m_x, m_y;

};