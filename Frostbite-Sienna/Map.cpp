#include "stdafx.h"
#include "Map.h"
#include "tinyxml2.h"

Map::Map()
{
	// Set inital Drawing Mode and load Segment Definitions
	drawingMode = SEGMENT_SELECTION;
	LoadSegmentDefinitions();

	layers = 3;
}

void Map::Update()
{
	switch (drawingMode)
	{
	case SEGMENT_SELECTION:

		


		break;
	case LEDGES:

		break;
	}
}

void Map::Draw(sf::RenderWindow &renderWindow)
{
	switch (drawingMode)
	{
	case SEGMENT_SELECTION:
		DrawSegmentSelection(renderWindow);
		break;
	case LEDGES:

		break;
	}
}

void Map::DrawMap(sf::RenderWindow &renderWindow)
{
	float scale = 1.0f;

	for (int l = 0; l < layers; ++l)
	{
		for (int i = 0; i < mapSeg.size(); i++)
		{
			if (mapSeg[i]->layer == l)
			{
				sf::Rect<float> dRect;
				dRect.left = (mapSeg[i]->location.x - scroll.x) * scale;
				dRect.top = (mapSeg[i]->location.y - scroll.y) * scale;
				dRect.width = (float)segDef[mapSeg[i]->segmentIndex]->width;
				dRect.height = (float)segDef[mapSeg[i]->segmentIndex]->height;

				sf::Sprite segSprite;
				segSprite.setTexture(segDef[mapSeg[i]->segmentIndex]->tex);
				segSprite.setPosition(dRect.left, dRect.top);
				segSprite.setScale(scale, scale);
				segSprite.setOrigin(segDef[mapSeg[i]->segmentIndex]->width / 2, segDef[mapSeg[i]->segmentIndex]->height / 2);
				segSprite.setRotation(mapSeg[i]->rotation);
				renderWindow.draw(segSprite);
			}
		}
	}
}

void Map::DrawSegmentSelection(sf::RenderWindow &renderWindow)
{
	sf::RectangleShape segmentShape;
	segmentShape.setPosition(975, 20);
	sf::Vector2<float> segmentSize(285, 680);
	segmentShape.setSize(segmentSize);
	segmentShape.setFillColor(sf::Color(255,255,255,200));
	renderWindow.draw(segmentShape);

	sf::Rect<int> dRect;

	int segCols = 250 / 45;
	int curCol = 0;
	int curRow = 0;

	// Draw segments
	for (int i = 0; i < segDef.size(); i++)
	{
		dRect.left = (985) + (55 * curCol);
		dRect.top = 30 + (curRow * 60);
		dRect.width = 45;
		dRect.height = 45;

		sf::Vector2u texSize = segDef[i]->tex.getSize();

		// Get sprite reference
		sf::Sprite segSprite;
		segSprite.setTexture(segDef[i]->tex);
		segSprite.scale((float)dRect.width / texSize.x, (float)dRect.height / texSize.y);
		segSprite.setPosition((float)dRect.left, (float)dRect.top);
		renderWindow.draw(segSprite);

		

		curCol++;
		if (curCol == segCols) {
			curCol = 0;
			curRow++;
		}
	}
}

int Map::AddSegment(int layer, int index)
{
	mapSeg.push_back(new MapSegment(layer, index));

	return -1;
}

void Map::LoadSegmentDefinitions()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile( "Tilesets/tileset01.xml" );

	tinyxml2::XMLElement* root = doc.FirstChildElement( "tileset" );
	for (tinyxml2::XMLElement* e = root->FirstChildElement( "tile" ); e; e = e->NextSiblingElement() )
	{
		// Todo add validation routines to stop buggy XML files being imported

		int tileId = std::atoi(e->Attribute("id"));
		std::string name = e->Attribute("name");
		std::string filepath = e->Attribute("image");

		segDef.push_back(new SegmentDefinition(name, filepath));
		
	}
}