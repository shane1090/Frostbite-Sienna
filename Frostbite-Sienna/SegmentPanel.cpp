#include "stdafx.h"
#include "SegmentPanel.h"

SegmentPanel::SegmentPanel(std::vector<SegmentDefinition*> &segDef, std::vector<MapSegment*> &mapSeg) : segDef(segDef), mapSeg(mapSeg)
{
	this->segDef = segDef;
	this->scrollRow = 0;
}


SegmentPanel::~SegmentPanel(void)
{

}

void SegmentPanel::Update(int curLayer, InputManager &input, std::vector<MapSegment*> &mapSeg)
{
	this->mapSeg = mapSeg;

	sf::Rect<int> dRect;

	int segCols = 250 / 45;
	int curCol = 0;
	int curRow = 0;

	int maxSegments = 16;
	int t = 0;

	// Draw segments
	for (int i = 0; i < segDef.size(); i++)
	{
		dRect.left = (985) + (55 * curCol);
		dRect.top = 30 + (curRow * 60);
		dRect.width = 45;
		dRect.height = 45;

		// This should be moved to Update
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (input.mousePos.x > dRect.left && input.mousePos.x < (dRect.left + dRect.width) &&
				input.mousePos.y > dRect.top && input.mousePos.y < (dRect.top + dRect.height))
			{
				AddSegment(curLayer, i);
			}
		}

		curCol++;
		if (curCol == segCols) {
			curCol = 0;
			curRow++;
		}
	}
}

void SegmentPanel::Draw(InputManager &input, sf::RenderWindow &Window)
{
	sf::RectangleShape segmentShape;
	segmentShape.setPosition(975, 20);
	sf::Vector2<float> segmentSize(285, 680);
	segmentShape.setSize(segmentSize);
	segmentShape.setFillColor(sf::Color(255,255,255,200));
	Window.draw(segmentShape);

	sf::Rect<int> dRect;

	int segCols = 250 / 45;
	int curCol = 0;
	int curRow = 0;

	int maxSegments = 16;
	int t = 0;

	// Draw segments
	for (int i = 0; i < segDef.size(); i++)
	{
		dRect.left = (985) + (55 * curCol);
		dRect.top = 30 + (curRow * 60);
		dRect.width = 45;
		dRect.height = 45;

		// Get sprite reference
		sf::Sprite segSprite;
		segSprite.setTexture(segDef[i]->tex);
		segSprite.scale((float)dRect.width / segDef[i]->width, (float)dRect.height / segDef[i]->height);
		segSprite.setPosition((float)dRect.left, (float)dRect.top);
		Window.draw(segSprite);

		curCol++;
		if (curCol == segCols) {
			curCol = 0;
			curRow++;
		}
	}
}

void SegmentPanel::AddSegment(int layer, int index)
{
	std::cout << "Segment Added to Map: " << index << std::endl; 
	mapSeg.push_back(new MapSegment(layer, index));
}