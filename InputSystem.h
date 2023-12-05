#pragma once
#include "InputListener.h"
//#include <map>
#include <unordered_set>
#include "Point.h"


class InputSystem
{
	InputSystem();
	~InputSystem();

public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point& position);

	void showCursor(bool flag);

	static InputSystem* get();
	static void create();
	static void release();

private:
	//std::map<InputListener*, InputListener*> m_map_listeners;
	std::unordered_set<InputListener*> m_set_listeners;
	
	/* keyboard */
	unsigned char m_keys_state[256] = {};
	unsigned char m_old_keys_state[256] = {};

	/* mouse */
	bool m_first_time_moved = true;  // cheks the update method's first call
	Point m_old_mouse_position;

	/* system */
	static InputSystem* m_system;
};

