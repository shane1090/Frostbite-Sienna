#pragma once
#include "SegmentDefinition.h"
#include "MapSegment.h"

class Map
{
public:
	Map();
	void Update();
	void Draw(sf::RenderWindow &renderWindow);
	void DrawMap(sf::RenderWindow &renderWindow);
	void DrawSegmentSelection(sf::RenderWindow &renderWindow);
	int AddSegment(int layer, int index);

private:
	int layers;

	void LoadSegmentDefinitions();
	std::vector<SegmentDefinition*> segDef;
	std::vector<MapSegment*> mapSeg;

	enum drawingMode_t { SEGMENT_SELECTION, LEDGES };
	drawingMode_t drawingMode;

	sf::Vector2<float> scroll;
};