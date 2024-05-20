#pragma once

class MathUtils
{
public:
	MathUtils()
	{

	}
	~MathUtils()
	{

	}

	static float lerp(float start, float end, float delta)
	{
		return start * (1.0f - delta) + end * delta;
	}
};