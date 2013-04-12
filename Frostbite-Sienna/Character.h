#pragma once

#include "Ledge.h"
#include "Map.h"
#include "InputManager.h"

class Character
{
public:
	Character(void);
	~Character(void);
	void Draw(sf::RenderWindow &Window, sf::Vector2<float> &scroll);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);

	Map *map;

private:
	sf::Vector2<float> location;
	sf::Vector2<float> trajectory;

	int ledgeAttach;

	float speed;

	enum CharState { GROUNDED, AIR };
	CharState state;

	enum LedgeFlags { SOLID };

	void Land();
};

