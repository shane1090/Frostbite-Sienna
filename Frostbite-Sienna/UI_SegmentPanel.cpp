#include "stdafx.h"
#include "UI_SegmentPanel.h"

SegmentPanel::SegmentPanel(std::vector<SegmentDefinition*> &segDef, std::vector<MapSegment*> &mapSeg) : segDef(segDef), mapSeg(mapSeg)
{
	this->scrollRow = 0;
	this->offset = 0;

	panelPos = sf::Vector2<int>(SCREEN_WIDTH - (MAX_SEGMENT_COLS * (MAX_SEGMENT_SIZE + SEGMENT_PADDING)) - SEGMENT_PADDING - 10, 
							  SCREEN_HEIGHT - (MAX_SEGMENT_ROWS * (MAX_SEGMENT_SIZE + SEGMENT_PADDING)) - SEGMENT_PADDING);

	segmentSize = sf::Vector2<float>((MAX_SEGMENT_COLS * (MAX_SEGMENT_SIZE + SEGMENT_PADDING)) + SEGMENT_PADDING + 10, 
								   (MAX_SEGMENT_ROWS * (MAX_SEGMENT_SIZE + SEGMENT_PADDING)) + SEGMENT_PADDING);
}


SegmentPanel::~SegmentPanel(void)
{

}

void SegmentPanel::Update(InputManager &input)
{
	if (input.mousePos.x > panelPos.x && input.mousePos.y > panelPos.y &&
		input.mousePos.x < 1280 && input.mousePos.y < 720)
	{
		if (input.MouseWheelMovedDown() && ((offset * MAX_SEGMENT_COLS) + MAX_SEGMENTS < segDef.size()))
		{
			offset++;
		} else if (input.MouseWheelMovedUp() && offset > 0)
		{
			offset--;
		}
	}
}

void SegmentPanel::Draw(int curLayer, sf::Vector2<float> scroll, InputManager &input, sf::RenderWindow &Window)
{
	sf::RectangleShape segmentShape;
	segmentShape.setPosition(panelPos.x, panelPos.y);	
	segmentShape.setSize(segmentSize);
	segmentShape.setFillColor(sf::Color(0,0,0,200));
	Window.draw(segmentShape);

	sf::Rect<int> dRect;
	int curCol = 0;
	int curRow = 0;
	int t = 0;

	// Draw segments
	for (int i = 0; i < segDef.size(); i++)
	{
		if (i >= (offset * MAX_SEGMENT_COLS) && t < MAX_SEGMENTS) {
			dRect.left = panelPos.x + ((MAX_SEGMENT_SIZE + SEGMENT_PADDING) * curCol) + SEGMENT_PADDING;
			dRect.top = panelPos.y + ((MAX_SEGMENT_SIZE + SEGMENT_PADDING) * curRow) + SEGMENT_PADDING;
			dRect.width = segDef[i]->width;
			dRect.height = segDef[i]->height;

			if (dRect.width > MAX_SEGMENT_SIZE)
			{
				float ratio = (float)MAX_SEGMENT_SIZE / (float)dRect.width;
				dRect.width = MAX_SEGMENT_SIZE;
				dRect.height = dRect.height * ratio;
			}

			if (dRect.height > MAX_SEGMENT_SIZE)
			{
				float ratio = (float)MAX_SEGMENT_SIZE / (float)dRect.height;
				dRect.width = dRect.width * ratio;
				dRect.height = MAX_SEGMENT_SIZE;
			}

			if (dRect.height < MAX_SEGMENT_SIZE)
				dRect.top = dRect.top + (MAX_SEGMENT_SIZE - dRect.height) / 2;

			if (dRect.width < MAX_SEGMENT_SIZE)
				dRect.left = dRect.left + (MAX_SEGMENT_SIZE - dRect.width) / 2;

			// Get sprite reference
			sf::Sprite segSprite;
			segSprite.setTexture(segDef[i]->tex);
			segSprite.scale((float)dRect.width / segDef[i]->width, (float)dRect.height / segDef[i]->height);
			segSprite.setPosition((float)dRect.left, (float)dRect.top);
			Window.draw(segSprite);

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
			if (curCol == MAX_SEGMENT_COLS) {
				curCol = 0;
				curRow++;
			}
		}
	}

	// Draw Scrollbar
	int rows = ceil((float)segDef.size() / (float)MAX_SEGMENT_COLS) - (MAX_SEGMENT_ROWS - 1);
	int scrollBarHeight = ceil(((MAX_SEGMENT_ROWS * (MAX_SEGMENT_SIZE + SEGMENT_PADDING)) + SEGMENT_PADDING) / rows);
	sf::Vector2<int> scrollPos(panelPos.x + ((MAX_SEGMENT_COLS * (MAX_SEGMENT_SIZE + SEGMENT_PADDING)) + SEGMENT_PADDING),
							   panelPos.y + (offset * scrollBarHeight));

	sf::RectangleShape scrollBarShape;
	scrollBarShape.setPosition(scrollPos.x, panelPos.y);
	sf::Vector2<float> scrollBarSize(10, (MAX_SEGMENT_ROWS * (MAX_SEGMENT_SIZE + SEGMENT_PADDING)) + SEGMENT_PADDING);
	scrollBarShape.setSize(scrollBarSize);
	scrollBarShape.setFillColor(sf::Color(46,70,109,200));
	Window.draw(scrollBarShape);
	
	sf::RectangleShape scrollBarHandleShape;
	scrollBarHandleShape.setPosition(scrollPos.x, scrollPos.y);
	sf::Vector2<float> scrollBarHandleSize(10, scrollBarHeight);
	scrollBarHandleShape.setSize(scrollBarHandleSize);
	scrollBarHandleShape.setFillColor(sf::Color(255,255,255,255));
	Window.draw(scrollBarHandleShape);
}

void SegmentPanel::AddSegment(int layer, int index, sf::Vector2<float> scroll)
{
	sf::Vector2<float> position = scroll + sf::Vector2<float>(640.0f, 360.0f);
	std::cout << "Segment Added to Map: " << index << std::endl; 
	mapSeg.push_back(new MapSegment(layer, index, position, 0, sf::Vector2<float>(1.0f,1.0f)));
}