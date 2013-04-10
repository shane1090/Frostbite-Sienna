#pragma once

class InputManager
{
public:
	void Poll(sf::RenderWindow &Window);

	void Update(sf::Event event);
	bool HeldDown(int value, bool mouse = false);
	bool Pressed(int value, bool mouse = false);
	bool Released(int value, bool mouse = false);
	int mouseWheel(){return mouseWheelClicks;}
	void ClearUnique();
	sf::Vector2i getMousePosition(){return mousePosition;}

private:
	std::set<int> down;
	std::set<int> mouseDown;
	std::set<int> pressedMouse;
	std::set<int> releasedMouse;
	std::set<int> pressedKeyboard;
	std::set<int> releasedKeyboard;
	sf::Vector2i mousePosition;
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