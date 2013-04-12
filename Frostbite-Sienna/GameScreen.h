#pragma once

#include "InputManager.h"

class GameScreen
{
public:
	GameScreen();
	~GameScreen();

	virtual void LoadContent();
	virtual void UnloadContent();
	virtual void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	virtual void Draw(sf::RenderWindow &window, sf::Clock &gameTime);

protected:

private:

};