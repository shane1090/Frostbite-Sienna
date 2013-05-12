#pragma once

class Part
{
public:
	Part(int index);
	~Part(void);

	sf::Vector2f location;
	sf::Vector2f scaling;
	float rotation;
	int index;
	int flip;

};

