#pragma once

#include "Map.h"
#include "Panel.h"

class UILedgePanel : public Panel
{
public:
	UILedgePanel(Map *&map, int &curLedge);
	~UILedgePanel(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);

private:
	Map *map;
	int& curLedge;
	sf::Text text;

};

