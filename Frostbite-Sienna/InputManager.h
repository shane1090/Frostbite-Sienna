#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update(sf::Event event);
	bool KeyPressed(sf::Keyboard::Key key);
	bool KeyPressed(std::vector<sf::Keyboard::Key> keys);
	bool KeyReleased(sf::Keyboard::Key key);
	bool KeyReleased(std::vector<sf::Keyboard::Key> keys);
	bool KeyDown(sf::RenderWindow &Window, sf::Keyboard::Key key);
	bool KeyDown(sf::RenderWindow &Window, std::vector<sf::Keyboard::Key> keys);

protected:
private:
	sf::Event event;
};

#endif // INPUTMANAGER_H