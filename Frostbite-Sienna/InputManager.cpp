#include "stdafx.h"
#include "InputManager.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

void InputManager::Update(sf::Event event)
{
	this->event = event;
}

bool InputManager::KeyPressed(sf::Keyboard::Key key)
{
	if(event.key.code == key && event.type == sf::Event::KeyPressed)
		return true;
	return false;
}

bool InputManager::KeyPressed(std::vector<sf::Keyboard::Key> keys)
{
	for (int i = 0; i < keys.size(); i++)
	{
		if (KeyPressed(keys[i]))
			return true;
	}
	return false;
}

bool InputManager::KeyReleased(sf::Keyboard::Key key)
{
	if (event.key.code == key && event.type == sf::Event::KeyReleased)
		return true;
	return false;
}

bool InputManager::KeyReleased(std::vector<sf::Keyboard::Key> keys)
{
	for (int i = 0; i < keys.size(); i++)
	{
		if (KeyReleased(keys[i]))
			return true;
	}
	return false;
}

bool InputManager::KeyDown(sf::RenderWindow &Window, sf::Keyboard::Key key)
{
	if (sf::Keyboard::isKeyPressed(key))
		return true;
	return false;
}

bool InputManager::KeyDown(sf::RenderWindow &Window, std::vector<sf::Keyboard::Key> keys)
{
	for (int i = 0; i < keys.size(); i++)
	{
		if (sf::Keyboard::isKeyPressed(keys[i]))
			return true;
	}
	return false;
}