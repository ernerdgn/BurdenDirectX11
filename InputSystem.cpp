#include "InputSystem.h"
#include <Windows.h>
#include <iostream>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
	/* MOUSE */
	POINT current_mouse_position = {};
	::GetCursorPos(&current_mouse_position);

	if (m_first_time_moved)
	{
		m_old_mouse_position = Point(current_mouse_position.x, current_mouse_position.y);
		m_first_time_moved = false;
	}

	if (current_mouse_position.x != m_old_mouse_position.m_x || current_mouse_position.y != m_old_mouse_position.m_y)  // if mouse moved
	{
		std::unordered_set<InputListener*>::iterator mouse_listener_iterator = m_set_listeners.begin();

		while (mouse_listener_iterator != m_set_listeners.end())
		{
			//std::cout << "\nX: " << current_mouse_position.x - m_old_mouse_position.m_x << "\nY: " << current_mouse_position.y - m_old_mouse_position.m_y;
			(*mouse_listener_iterator)->onMouseMove(Point(
				current_mouse_position.x, 
				current_mouse_position.y));
			++mouse_listener_iterator;
		}
	}

	m_old_mouse_position = Point(current_mouse_position.x, current_mouse_position.y);

	/* KEYBOARD NAD MOUSE BUTTONS */
	if (::GetKeyboardState(m_keys_state))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			//bitmask
			if (m_keys_state[i] & 0x80)  //key/button pressed
			{
				//std::map<InputListener*, InputListener*>::iterator listener_iterator = m_map_listeners.begin();
				std::unordered_set<InputListener*>::iterator keys_listener_iterator = m_set_listeners.begin();

				while (keys_listener_iterator != m_set_listeners.end())
				{
					if (i == VK_LBUTTON)
					{
						if (m_keys_state[i] != m_old_keys_state[i])
							(*keys_listener_iterator)->onLeftMouseButtonDown(Point(current_mouse_position.x, current_mouse_position.y));
					}

					else if (i == VK_RBUTTON)
					{
						if (m_keys_state[i] != m_old_keys_state[i])
							(*keys_listener_iterator)->onRightMouseButtonDown(Point(current_mouse_position.x, current_mouse_position.y));
					}
					
					else
					{
						(*keys_listener_iterator)->onKeyDown(i);
					}
					++keys_listener_iterator;
				}
			}
			else  //key up
			{
				if (m_keys_state[i] != m_old_keys_state[i])
				{
					//std::map<InputListener*, InputListener*>::iterator listener_iterator = m_map_listeners.begin();
					std::unordered_set<InputListener*>::iterator keys_listener_iterator = m_set_listeners.begin();

					while (keys_listener_iterator != m_set_listeners.end())
					{
						if (i == VK_LBUTTON)
							(*keys_listener_iterator)->onLeftMouseButtonUp(Point(current_mouse_position.x, current_mouse_position.y));
						else if (i == VK_RBUTTON)
							(*keys_listener_iterator)->onRightMouseButtonUp(Point(current_mouse_position.x, current_mouse_position.y));
						else
							(*keys_listener_iterator)->onKeyUp(i);
						
						++keys_listener_iterator;
					}
				}
			}
		}
		//current key state to old key state buffer storage
		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener)
{
	//m_map_listeners.insert(std::make_pair<InputListener*, InputListener*>(
	//	std::forward<InputListener*>(listener), std::forward<InputListener*>(listener)
	//));
	m_set_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	//std::map<InputListener*, InputListener*>::iterator listener_iterator = m_map_listeners.find(listener);

	//if (listener_iterator != m_map_listeners.end()) m_map_listeners.erase(listener_iterator);
	m_set_listeners.erase(listener);
}

void InputSystem::setCursorPosition(const Point& position)
{
	::SetCursorPos(position.m_x, position.m_y);
}

void InputSystem::showCursor(bool flag)
{
	::ShowCursor(flag);
}

InputSystem* InputSystem::get()
{
	static InputSystem input_system;
	return &input_system;
}