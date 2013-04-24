#include "stdafx.h"
#include "UI_SegmentPanel.h"

UISegmentPanel::UISegmentPanel(Map *&map, int &tile) : tile(tile), Panel()
{
	this->map = map;
	this->position = sf::Rect<float>(220,85,830,575);
	this->title = "Segments";
	this->isResizable = false;
	this->offset = 0;

	// Set scrolling min and max heights
	sMin = 0;
	sMax = (ceil((float)map->segDef.size() / 8.f) * (P_SEGMENT_PADDING + P_MAX_SEGMENT_SIZE) + P_SEGMENT_PADDING) - 549.f;
	sPanelHeight = 549;
	sTargetHeight = ceil((float)map->segDef.size() / 8.f) * (P_SEGMENT_PADDING + P_MAX_SEGMENT_SIZE) + P_SEGMENT_PADDING;
}

UISegmentPanel::~UISegmentPanel(void)
{

}

void UISegmentPanel::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	Panel::Update(Window, gameTime);

	sf::FloatRect panelRect((position.left + 205) / 1280.f,
							(position.top + 26) / 720.f,
							(position.width - 215) / 1280.f,
							(position.height - 26) / 720.f);

	panelView.reset(sf::FloatRect(
		(position.left + 205),
		(position.top + 26),
		(position.width - 215),
		(position.height - 26)));
	
	panelView.setViewport(panelRect);
}

void UISegmentPanel::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

	Window.setView(panelView);

	// Then panel specific items on top
	sf::Rect<int> dRect;
	int curCol = 0;
	int curRow = 0;
	int t = 0;

	// Draw segments
	for (int i = 0; i < map->segDef.size(); i++)
	{
		//if (i >= (offset * P_MAX_SEGMENT_COLS) && t < P_MAX_SEGMENTS) {
			dRect.left = (position.left + 205) + ((P_MAX_SEGMENT_SIZE + P_SEGMENT_PADDING) * curCol) + P_SEGMENT_PADDING;
			dRect.top = (position.top + 25) + ((P_MAX_SEGMENT_SIZE + P_SEGMENT_PADDING) * curRow) + P_SEGMENT_PADDING - offset;
			dRect.width = map->segDef[i]->width;
			dRect.height = map->segDef[i]->height;

			sf::Vector2f itemPos(dRect.left, dRect.top);

			if (dRect.width > P_MAX_SEGMENT_SIZE)
			{
				float ratio = (float)P_MAX_SEGMENT_SIZE / (float)dRect.width;
				dRect.width = P_MAX_SEGMENT_SIZE;
				dRect.height = dRect.height * ratio;
			}

			if (dRect.height > P_MAX_SEGMENT_SIZE)
			{
				float ratio = (float)P_MAX_SEGMENT_SIZE / (float)dRect.height;
				dRect.width = dRect.width * ratio;
				dRect.height = P_MAX_SEGMENT_SIZE;
			}

			if (dRect.height < P_MAX_SEGMENT_SIZE)
				dRect.top = dRect.top + (P_MAX_SEGMENT_SIZE - dRect.height) / 2;

			if (dRect.width < P_MAX_SEGMENT_SIZE)
				dRect.left = dRect.left + (P_MAX_SEGMENT_SIZE - dRect.width) / 2;

			// Get sprite reference
			sf::Sprite segSprite;
			segSprite.setTexture(map->segDef[i]->tex);
			segSprite.scale((float)dRect.width / map->segDef[i]->width, (float)dRect.height / map->segDef[i]->height);
			segSprite.setPosition((float)dRect.left, (float)dRect.top);

			if (mousePos.x > (position.left + 205) && mousePos.x < (position.left + position.width) &&
				mousePos.y > (position.top + 26) && mousePos.y < (position.top + position.height))
			{
				if (mousePos.x > itemPos.x && mousePos.x < (itemPos.x + P_MAX_SEGMENT_SIZE) &&
					mousePos.y > itemPos.y && mousePos.y < (itemPos.y + P_MAX_SEGMENT_SIZE))
				{
					// Draw panel background
					sf::RectangleShape hoverRect;
					hoverRect.setPosition(itemPos.x, itemPos.y);
					hoverRect.setSize(sf::Vector2f(P_MAX_SEGMENT_SIZE, P_MAX_SEGMENT_SIZE));
					hoverRect.setFillColor(sf::Color(255,255,255,255));
					Window.draw(hoverRect);

					// This should be moved to Update
					if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
					{
						//SetSegment(i);
						tile = i;
						std::cout << "Tile set to: " << i << std::endl;
						this->minimized = true;
					}
				}
			}

			Window.draw(segSprite);

			t++;
			curCol++;
			if (curCol == P_MAX_SEGMENT_COLS) {
				curCol = 0;
				curRow++;
			}
		//}
	}

	Window.setView(Window.getDefaultView());
}