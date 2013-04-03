#include "stdafx.h"
#include "SegmentPanel.h"

SegmentPanel::SegmentPanel(std::vector<SegmentDefinition*> &segDef, std::vector<MapSegment*> &mapSeg) : segDef(segDef), mapSeg(mapSeg)
{
	this->segDef = segDef;
	this->scrollRow = 0;
	this->offset = 0;
	this->maxSegments = 55;
}


SegmentPanel::~SegmentPanel(void)
{

}

void SegmentPanel::Update(int curLayer, InputManager &input, sf::Vector2<float> scroll)
{

	sf::Rect<int> dRect;
	int curCol = 0;
	int curRow = 0;
	int t = 0;

	// Draw segments
	for (int i = 0; i < segDef.size(); i++)
	{
		if (i >= (offset * 5) && t < maxSegments) {
			dRect.left = (985) + (55 * curCol);
			dRect.top = 30 + (curRow * 60);
			dRect.width = 45;
			dRect.height = 45;

			// This should be moved to Update
			if (input.MouseButtonPressed(sf::Mouse::Button::Left))
			{
				if (input.mousePos.x > dRect.left && input.mousePos.x < (dRect.left + dRect.width) &&
					input.mousePos.y > dRect.top && input.mousePos.y < (dRect.top + dRect.height))
				{
					AddSegment(curLayer, i, scroll);
				}
			}

			t++;
			curCol++;
			if (curCol == 5) {
				curCol = 0;
				curRow++;
			}
		}
	}

	if (input.mousePos.x > 975 && input.mousePos.y > 20 &&
		input.mousePos.x < 1260 && input.mousePos.y < 700)
	{
		if (input.MouseWheelMovedDown() && ((offset * 5) + 55 < segDef.size()))
		{
			offset++;
		} else if (input.MouseWheelMovedUp() && offset > 0)
		{
			offset--;
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
	int curCol = 0;
	int curRow = 0;
	int t = 0;

	// Draw segments
	for (int i = 0; i < segDef.size(); i++)
	{
		if (i >= (offset * 5) && t < maxSegments) {
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

			t++;
			curCol++;
			if (curCol == 5) {
				curCol = 0;
				curRow++;
			}
		}
	}
}

void SegmentPanel::AddSegment(int layer, int index, sf::Vector2<float> scroll)
{
	sf::Vector2<float> position = scroll + sf::Vector2<float>(640.0f, 360.0f);
	std::cout << "Segment Added to Map: " << index << std::endl; 
	mapSeg.push_back(new MapSegment(layer, index, position, 0));
}