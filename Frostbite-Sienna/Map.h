#pragma once

#include "SegmentDefinition.h"
#include "MapSegment.h"
#include "Ledge.h"

class Map
{
public:
	Map(void);
	~Map(void);

	int GetLedgeSec(int l, float x);
	float GetLedgeYLoc(int l, int i, float x);
	void LoadConfig(std::vector<SegmentDefinition*> segDef, std::vector<MapSegment*> mapSeg, std::vector<Ledge*> ledges);
	void Draw(sf::RenderWindow &Window, int startLayer, int endLayer, sf::Vector2<float> &scroll);

	std::vector<SegmentDefinition*> segDef;
	std::vector<MapSegment*> mapSeg;
	std::vector<Ledge*> ledges;

private:
	float zoomScale;
};

