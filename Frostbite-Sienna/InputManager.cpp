#include "stdafx.h"
#include "InputManager.h"

void InputManager::Poll(sf::RenderWindow &Window)
{
	ClearUnique();

	mouseWheelClicks = 0;

	// Process events
	sf::Event event;
	while (Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			Window.close();
		}

		Update(event);
	}
}

void InputManager::Update(sf::Event event)
{
	currentKey = event.KeyPressed;

	if (event.type == sf::Event::MouseMoved)
	{
		mousePosition.x = event.mouseMove.x;
		mousePosition.y = event.mouseMove.y;
		return;
	}

	// Keyboard events
	if (event.type == sf::Event::KeyPressed)
	{
		pressedKeyboard.insert(event.key.code); // single event
		down.insert(event.key.code);
		return;
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		releasedKeyboard.insert(event.key.code); // single event
		down.erase(event.key.code);
		return;
	}

	// Mouse events
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		pressedMouse.insert(event.mouseButton.button); // single event
		mouseDown.insert(event.mouseButton.button);
		return;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		releasedMouse.insert(event.mouseButton.button); // single event
		mouseDown.erase(event.mouseButton.button);
		return;
	}
	else if (event.type == sf::Event::MouseWheelMoved)
	{
		mouseWheelClicks = event.mouseWheel.delta; // number of clicks
	}

	// Gamepad events
	else if (event.type == sf::Event::JoystickButtonPressed)
	{
		pressedButton.insert(event.joystickButton.button); // single event
		buttonDown.insert(event.joystickButton.button);
		return;
	}
	else if (event.type == sf::Event::JoystickButtonReleased)
	{
		releasedButton.insert(event.joystickButton.button); // single event
		buttonDown.erase(event.joystickButton.button);
		return;
	}

	//std::cout << "Button: " << event.joystickButton.button << std::endl;
}

bool InputManager::Pressed(int value, bool mouse, bool gamepad) // default false
{
	if (!gamepad)
	{
		if (!mouse)
		{
			if (pressedKeyboard.find(value) == pressedKeyboard.end())
				return false;
			else return true;
		}
		else if (pressedMouse.find(value) == pressedMouse.end())
			return false;
		else return true;
	}
	else if (pressedButton.find(value) == pressedButton.end())
		return false;
	else return true;
}

bool InputManager::Released(int value, bool mouse, bool gamepad) // default false
{
	if (!gamepad)
	{
		if (!mouse)
		{
			if (releasedKeyboard.find(value) == releasedKeyboard.end())
				return false;
			else return true;
		}
		else if (releasedMouse.find(value) == releasedMouse.end())
			return false;
		else return true;
	} 
	else if (releasedButton.find(value) == releasedButton.end())
		return false;
	else return true;
}

float InputManager::GetJoystickAxis(sf::Joystick::Axis axis)
{
	return sf::Joystick::getAxisPosition(1, sf::Joystick::X);
}

void InputManager::ClearUnique()
{
	pressedKeyboard.clear();
	releasedKeyboard.clear();
	pressedMouse.clear();
	releasedMouse.clear();
	pressedButton.clear();
	releasedButton.clear();
}

bool InputManager::HeldDown(int value, bool mouse, bool gamepad)
{
	if (!gamepad)
	{
		if (!mouse)
		{
			if (down.find(value) == down.end())
			{
				return false;
			}
			else return true;
		}
		else if (mouseDown.find(value) == mouseDown.end())
			return false;
		else return true;
	} else if (buttonDown.find(value) == buttonDown.end())
		return false;
	else return true;
}