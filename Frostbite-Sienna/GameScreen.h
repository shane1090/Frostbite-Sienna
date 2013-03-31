#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "InputManager.h"

class GameScreen
{
public:
	GameScreen();
	~GameScreen();

	virtual void LoadContent();
	virtual void UnloadContent();
	virtual void Update(sf::RenderWindow &Window, sf::Event event);
	virtual void Draw(sf::RenderWindow &window);

protected:
	InputManager input;
	std::vector<sf::Keyboard::Key> keys;
private:

};

#endif // GAMESCREEN_H