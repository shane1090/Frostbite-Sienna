#include "stdafx.h"
#include "UI_LedgePanel.h"


LedgePanel::LedgePanel(std::vector<Ledge*> &ledges) : ledges(ledges)
{
	panelRect = sf::Rect<float>(985, 420, 295, 300);
	offset = 0;
}


LedgePanel::~LedgePanel(void)
{

}

void LedgePanel::LoadContent(sf::Font &font)
{
	this->font = font;
}

void LedgePanel::Update()
{
	mousePos = InputManager::instance().getMousePosition();

	if (mousePos.x > panelRect.left && mousePos.y > panelRect.top &&
		mousePos.x < 1280 && mousePos.y < 720)
	{
		if ((InputManager::instance().mouseWheel() < 0) && (offset + MAX_LEDGE_ROWS < ledges.size()))
		{
			offset++;
		} else if ((InputManager::instance().mouseWheel() > 0) && offset > 0)
		{
			offset--;
		}
	}
}

void LedgePanel::Draw(sf::RenderWindow &Window, int &curLedge)
{
	sf::RectangleShape panelShape;
	panelShape.setPosition(panelRect.left, panelRect.top);
	sf::Vector2<float> panelSize(panelRect.width, panelRect.height);
	panelShape.setSize(panelSize);
	panelShape.setFillColor(sf::Color(0,0,0,200));
	Window.draw(panelShape);

	sf::Color tColor;

	int curRow = 0;
	int t = 0;

	for (int i = 0; i < ledges.size(); i++)
	{
		if (i >= offset && t < MAX_LEDGE_ROWS) {
			float y = t * 20;
		
			text.setString("Ledge Group " + Convert(i + 1));
			text.setPosition(panelRect.left + 10, panelRect.top + 10 + y);
			text.setFont(font);
			text.setCharacterSize(14);

			if (curLedge == i)
				tColor = sf::Color::Yellow;
			else
				tColor = sf::Color::White;

			// This should be moved to Update
			if (mousePos.x > panelRect.left + 10 && mousePos.x < panelRect.left + 120 &&
				mousePos.y > panelRect.top + 10 + y && mousePos.y < panelRect.top + 10 + y + 14)
			{
				tColor = sf::Color::Red;

				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					curLedge = i;
				}
			}

			text.setColor(tColor);
			Window.draw(text);

			text.setString("n " + Convert(ledges[i]->nodes.size()));
			text.setPosition(panelRect.left + 155, panelRect.top + 10 + y);
			text.setFont(font);
			text.setCharacterSize(14);
			text.setColor(tColor);
			Window.draw(text);

			text.setString("f " + Convert(ledges[i]->flags));
			text.setPosition(panelRect.left + 230, panelRect.top + 10 + y);
			text.setFont(font);
			text.setCharacterSize(14);
			text.setColor(tColor);
			Window.draw(text);

			// This should be moved to Update
			if (mousePos.x > panelRect.left + 230 && mousePos.x < panelRect.left + 285 &&
				mousePos.y > panelRect.top + 10 + y && mousePos.y < panelRect.top + 10 + y + 14)
			{
				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
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
	float rowheight = ((100.0f / rows) / 100.0f) * panelRect.height;

	scrollBarHeight = rowheight * (float)MAX_LEDGE_ROWS;
	
	sf::Vector2<int> scrollPos(SCREEN_WIDTH - 10,
							   panelRect.top + (offset * rowheight));

	sf::RectangleShape scrollBarShape;
	scrollBarShape.setPosition(scrollPos.x, panelRect.top);
	sf::Vector2<float> scrollBarSize(10, panelRect.height);
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