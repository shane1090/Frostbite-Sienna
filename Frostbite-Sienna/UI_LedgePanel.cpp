#include "stdafx.h"
#include "UI_LedgePanel.h"


LedgePanel::LedgePanel(std::vector<Ledge*> &ledges) : ledges(ledges)
{
	panel = sf::Rect<float>(985, 420, 295, 300);
	offset = 0;
}


LedgePanel::~LedgePanel(void)
{

}

void LedgePanel::LoadContent(sf::Font &font)
{
	this->font = font;
}

void LedgePanel::Update(InputManager &input)
{
	if (input.mousePos.x > panel.left && input.mousePos.y > panel.top &&
		input.mousePos.x < 1280 && input.mousePos.y < 720)
	{
		if (input.MouseWheelMovedDown() && (offset + MAX_LEDGE_ROWS < ledges.size()))
		{
			offset++;
		} else if (input.MouseWheelMovedUp() && offset > 0)
		{
			offset--;
		}
	}
}

void LedgePanel::Draw(sf::RenderWindow &Window, int &curLedge, InputManager &input)
{
	sf::RectangleShape panelShape;
	panelShape.setPosition(panel.left, panel.top);
	sf::Vector2<float> panelSize(panel.width, panel.height);
	panelShape.setSize(panelSize);
	panelShape.setFillColor(sf::Color(0,0,0,200));
	Window.draw(panelShape);

	sf::Color tColor;

	int curRow = 0;
	int t = 0;

	for (int i = 0; i < ledges.size(); i++)
	{
		if (i >= (offset * MAX_LEDGE_ROWS) && t < MAX_LEDGE_ROWS) {
			float y = t * 20;
		
			text.setString("Ledge Group " + Convert(i + 1));
			text.setPosition(panel.left + 10, panel.top + 10 + y);
			text.setFont(font);
			text.setCharacterSize(14);

			if (curLedge == i)
				tColor = sf::Color::Yellow;
			else
				tColor = sf::Color::White;

			// This should be moved to Update
			if (input.mousePos.x > panel.left + 10 && input.mousePos.x < panel.left + 120 &&
				input.mousePos.y > panel.top + 10 + y && input.mousePos.y < panel.top + 10 + y + 14)
			{
				tColor = sf::Color::Red;

				if (input.MouseButtonPressed(sf::Mouse::Button::Left))
				{
					curLedge = i;
				}
			}

			text.setColor(tColor);
			Window.draw(text);

			text.setString("n " + Convert(ledges[i]->nodes.size()));
			text.setPosition(panel.left + 155, panel.top + 10 + y);
			text.setFont(font);
			text.setCharacterSize(14);
			text.setColor(tColor);
			Window.draw(text);

			text.setString("f " + Convert(ledges[i]->flags));
			text.setPosition(panel.left + 230, panel.top + 10 + y);
			text.setFont(font);
			text.setCharacterSize(14);
			text.setColor(tColor);
			Window.draw(text);

			// This should be moved to Update
			if (input.mousePos.x > panel.left + 230 && input.mousePos.x < panel.left + 285 &&
				input.mousePos.y > panel.top + 10 + y && input.mousePos.y < panel.top + 10 + y + 14)
			{
				if (input.MouseButtonPressed(sf::Mouse::Button::Left))
				{
					curLedge = i;
					ledges[i]->flags = (ledges[i]->flags + 1) % 2;
				}
			}

			t++;
			curRow++;
		}
	}

	// Draw Scrollbar
	int rows = ledges.size();
	int scrollBarHeight = ceil((MAX_LEDGE_ROWS * 20) + 20 / rows);
	sf::Vector2<int> scrollPos(SCREEN_WIDTH - 10,
							   panel.top + (offset * scrollBarHeight));

	sf::RectangleShape scrollBarShape;
	scrollBarShape.setPosition(scrollPos.x, panel.top);
	sf::Vector2<float> scrollBarSize(10, panel.height);
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