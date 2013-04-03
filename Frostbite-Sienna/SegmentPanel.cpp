#include "stdafx.h"
#include "SegmentPanel.h"

SegmentPanel::SegmentPanel(std::vector<SegmentDefinition*> &segDef, std::vector<MapSegment*> &mapSeg) : segDef(segDef), mapSeg(mapSeg)
{
	this->segDef = segDef;
	this->scrollRow = 0;
	this->offset = 0;
	this->maxSegments = 50;
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
			dRect.left = (1005) + (55 * curCol);
			dRect.top = 80 + (curRow * 60);
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

	if (input.mousePos.x > 995 && input.mousePos.y > 60 &&
		input.mousePos.x < 1280 && input.mousePos.y < 700)
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
	segmentShape.setPosition(995, 60);
	sf::Vector2<float> segmentSize(285, 640);
	segmentShape.setSize(segmentSize);
	segmentShape.setFillColor(sf::Color(0,0,0,180));
	Window.draw(segmentShape);

	sf::Rect<int> dRect;
	int curCol = 0;
	int curRow = 0;
	int t = 0;

	// Draw segments
	for (int i = 0; i < segDef.size(); i++)
	{
		if (i >= (offset * 5) && t < maxSegments) {
			dRect.left = (1005) + (55 * curCol);
			dRect.top = 80 + (curRow * 60);
			dRect.width = segDef[i]->width;
			dRect.height = segDef[i]->height;

			if (dRect.width > 45)
			{
				float ratio = 45.0f / (float)dRect.width;
				dRect.width = 45;
				dRect.height = dRect.height * ratio;
			}

			if (dRect.height > 45)
			{
				float ratio = 45.0f / (float)dRect.height;
				dRect.width = dRect.width * ratio;
				dRect.height = 45;
			}

			if (dRect.height < 45)
				dRect.top = dRect.top + (45 - dRect.height) / 2;

			if (dRect.width < 45)
				dRect.left = dRect.left + (45 - dRect.width) / 2;

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