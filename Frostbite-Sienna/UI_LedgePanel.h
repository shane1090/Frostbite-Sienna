#pragma once

#include "Ledge.h"
#include "InputManager.h"

class LedgePanel
{
public:
	LedgePanel(std::vector<Ledge*> &ledges);
	~LedgePanel(void);

	void LoadContent(sf::Font &font);
	void Update(InputManager &input);
	void Draw(sf::RenderWindow &Window, int &curLedge, InputManager &input);

	sf::Rect<float> panel;

private:
	std::vector<Ledge*> &ledges;

	sf::Font font;
	sf::Text text;

	int offset;
};

const int MAX_LEDGE_ROWS = 14;

