#pragma once

class InputManager
{
public:
	void Poll(sf::RenderWindow &Window);

	void Update(sf::Event event);
	bool HeldDown(int value, bool mouse = false, bool gamepad = false);
	bool Pressed(int value, bool mouse = false, bool gamepad = false);
	bool Released(int value, bool mouse = false, bool gamepad = false);
	float GetJoystickAxis(sf::Joystick::Axis axis);
	int mouseWheel(){return mouseWheelClicks;}
	void ClearUnique();
	sf::Vector2i getMousePosition(){return mousePosition;}
	char GetCurrentKey();

private:
	std::set<int> down;
	std::set<int> mouseDown;
	std::set<int> buttonDown;
	std::set<int> pressedMouse;
	std::set<int> releasedMouse;
	std::set<int> pressedKeyboard;
	std::set<int> releasedKeyboard;
	std::set<int> pressedButton;
	std::set<int> releasedButton;
	sf::Vector2i mousePosition;
	char currentKey;
	int mouseWheelClicks;

public:
	static InputManager& instance()
	{
		static InputManager _instance;
		return _instance;
	}

private:
	InputManager () {};
	
	InputManager (const InputManager&);
	~InputManager () {};

};