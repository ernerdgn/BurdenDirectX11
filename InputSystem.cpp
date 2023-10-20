#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
	if (::GetKeyboardState(m_keys_state))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			//bitmask
			if (m_keys_state[i] && 0x80)  //key pressed
			{
				std::map<InputListener*, InputListener*>::iterator listener_iterator = m_map_listeners.begin();

				while (listener_iterator != m_map_listeners.end())
				{
					listener_iterator->second->onKeyDown(i);
					++listener_iterator;
				}
			}
			else  //key up
			{
				if (m_keys_state[i] != m_old_keys_state[i])
				{
					std::map<InputListener*, InputListener*>::iterator listener_iterator = m_map_listeners.begin();

					while (listener_iterator != m_map_listeners.end())
					{
						listener_iterator->second->onKeyUp(i);
						++listener_iterator;
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
	m_map_listeners.insert(std::make_pair<InputListener*, InputListener*>(
		std::forward<InputListener*>(listener), std::forward<InputListener*>(listener)
	));
}

void InputSystem::removeListener(InputListener* listener)
{
	std::map<InputListener*, InputListener*>::iterator listener_iterator = m_map_listeners.find(listener);

	if (listener_iterator != m_map_listeners.end()) m_map_listeners.erase(listener_iterator);
}

InputSystem* InputSystem::get()
{
	static InputSystem input_system;
	return &input_system;
}
