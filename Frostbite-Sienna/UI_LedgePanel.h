#pragma once

#include "Ledge.h"
#include "InputManager.h"

class LedgePanel
{
public:
	LedgePanel(std::vector<Ledge*> &ledges);
	~LedgePanel(void);

	void LoadContent(sf::Font &font);
	void Update();
	void Draw(sf::RenderWindow &Window, int &curLedge);

	sf::Rect<float> panelRect;

private:
	std::vector<Ledge*> &ledges;

	sf::Font font;
	sf::Text text;

	int offset;

	float scrollBarHeight;

	sf::Vector2<int> mousePos;
};

const int MAX_LEDGE_ROWS = 14;

