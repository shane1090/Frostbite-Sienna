#pragma once
#include "Panel.h"
#include "Map.h"

class UISegmentPanel : public Panel
{
public:
	UISegmentPanel(Map *&map, int &tile);
	~UISegmentPanel(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);

private:
	Map *map;
	int& tile;
};

const int P_MAX_SEGMENT_COLS = 8;
const int P_MAX_SEGMENT_ROWS = 7;
const int P_MAX_SEGMENT_SIZE = 60;
const int P_SEGMENT_PADDING = 15;
const int P_MAX_SEGMENTS = P_MAX_SEGMENT_COLS * P_MAX_SEGMENT_ROWS;

