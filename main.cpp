#include "SpaceShooterGame.h"
#include "InputSystem.h"
#include <iostream>

int main()
{
	try
	{
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (...) { return -1; }

	{
		try
		{
			SpaceShooterGame app;
			std::cout << "Program is about to start..." << std::endl;
			while (app.is_running());
		}
		catch (...) 
		{
			InputSystem::release();
			GraphicsEngine::release();
			std::cout << "Starting failed. See the GraphicsEngine header for error code map." << std::endl;

			return -1;
		}
	}

	InputSystem::release();
	GraphicsEngine::release();

	std::cout << "Program closed without any errors." << std::endl;

	return 0;
}