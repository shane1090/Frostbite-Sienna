#pragma once

#include "Map.h"
#include "Panel.h"

class UILayerPanel : public Panel
{
public:
	UILayerPanel(Map *&map, int &curLayer);
	~UILayerPanel(void);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);

private:
	Map *map;
	int& curLayer;
	sf::Text text;
};

