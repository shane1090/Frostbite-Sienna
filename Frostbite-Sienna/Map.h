#pragma once

#include "SegmentDefinition.h"
#include "MapSegment.h"
#include "Ledge.h"
#include "Layer.h"

class Map
{
public:
	Map(void);
	~Map(void);

	int GetLedgeSec(int l, float x);
	float GetLedgeYLoc(int l, int i, float x);
	void LoadSegmentDefinitions();
	void LoadDefaultLayers();
	void LoadMap(std::string filePath);
	void SaveMap(std::string filePath);
	void LoadConfig(std::vector<SegmentDefinition*> segDef, std::vector<MapSegment*> mapSeg, std::vector<Ledge*> ledges);
	void Draw(sf::RenderWindow &Window, int startLayer, int endLayer, sf::Vector2<float> &scroll);
	void DrawLedges(sf::RenderWindow &Window, sf::Vector2<float> &scroll, int curLedge);

	std::vector<SegmentDefinition*> segDef;
	std::vector<MapSegment*> mapSeg;
	std::vector<Ledge*> ledges;
	std::vector<Layer*> layers;

private:
	
};

