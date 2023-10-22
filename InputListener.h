#pragma once
#include "Point.h"

class InputListener
{
public:
	InputListener()
	{

	}
	~InputListener()
	{

	}

	/* KEYBOARD */
	//pure virtual callback
	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;

	/* MOUSE */
	//pure virtual callback
	virtual void onMouseMove(const Point& delta_mouse_position) = 0;

	virtual void onLeftMouseButtonDown(const Point& mouse_position) = 0;
	virtual void onLeftMouseButtonUp(const Point& mouse_position) = 0;

	virtual void onRightMouseButtonDown(const Point& mouse_position) = 0;
	virtual void onRightMouseButtonUp(const Point& mouse_position) = 0;
};