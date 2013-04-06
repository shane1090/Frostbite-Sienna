#pragma once

#include "SegmentDefinition.h"
#include "MapSegment.h"
#include "InputManager.h"

class SegmentInfoPanel
{
public:
	SegmentInfoPanel(std::vector<SegmentDefinition*> &segDef, std::vector<MapSegment*> &mapSeg);
	~SegmentInfoPanel(void);
	void Draw(sf::RenderWindow &Window, int curSelectedSegment);
	void LoadContent(sf::Font &font);
	void Update();

private:
	std::vector<SegmentDefinition*> &segDef;
	std::vector<MapSegment*> &mapSeg;

	sf::Font font;
	sf::Text text;

	sf::Vector2<int> panelPos;
	sf::Vector2<float> segmentSize;
};

