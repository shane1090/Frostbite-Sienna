#include "stdafx.h"
#include "UI_LedgePanel.h"


UILedgePanel::UILedgePanel(Map *&map, int &curLedge) : curLedge(curLedge), Panel()
{
	this->map = map;
	this->title = "Ledges";
	this->position = sf::Rect<float>(1020,410,250,300);
	this->isResizable = false;

	position.height = 45 + (map->ledges.size() * 20);
}


UILedgePanel::~UILedgePanel(void)
{

}

void UILedgePanel::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	Panel::Update(Window, gameTime);
	
	position.height = 45 + (map->ledges.size() * 20);
}

void UILedgePanel::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

	if (map->ledges.size() > 0)
	{
		// Draw ledge info
		for (int i = 0; i < map->ledges.size(); i++)
		{
			
			sf::CircleShape circle;
			circle.setRadius(5);
			circle.setOutlineColor(sf::Color::White);
			circle.setOutlineThickness(1);
			circle.setFillColor(sf::Color(94, 103, 113, 255));
			circle.setPosition(position.left + 10, position.top + 35 + (20 * i) + 3);
			Window.draw(circle);

			if (curLedge == i)
			{
				sf::CircleShape circle;
				circle.setRadius(3);
				circle.setFillColor(sf::Color::White);
				circle.setPosition(position.left + 12, position.top + 35 + (20 * i) + 5);
				Window.draw(circle);
			}

			// This should be moved to Update
			if (mousePos.x > position.left && mousePos.x < position.left + position.width &&
				mousePos.y > position.top + 35 + (20 * i) && mousePos.y < position.top + 35 + (20 * i) + 14)
			{
				if (curLedge != i)
				{
					sf::CircleShape circle;
					circle.setRadius(3);
					circle.setFillColor(sf::Color::White);
					circle.setPosition(position.left + 12, position.top + 35 + (20 * i) + 5);
					Window.draw(circle);
				}

				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					curLedge = i;
				}
			}
			
			text.setString("Ledge Group " + Convert(i));
			text.setPosition(position.left + 30, position.top + 35 + (20 * i));
			text.setFont(font);
			text.setCharacterSize(12);
			text.setColor(sf::Color::White);
			Window.draw(text);

			text.setString("n " + Convert(map->ledges[i]->nodes.size()));
			text.setPosition(position.left + 155, position.top + 35 + (20 * i));
			text.setFont(font);
			text.setCharacterSize(12);
			text.setColor(sf::Color::White);
			Window.draw(text);

			text.setString("f " + Convert(map->ledges[i]->flags));
			text.setPosition(position.left + 215, position.top + 35 + (20 * i));
			text.setFont(font);
			text.setCharacterSize(12);
			text.setColor(sf::Color::White);
			Window.draw(text);

			// This should be moved to Update
			if (mousePos.x > position.left + 215 && mousePos.x < position.left + position.width &&
				mousePos.y > position.top + 35 + (20 * i) && mousePos.y < position.top + 35 + (20 * i) + 14)
			{
				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					curLedge = i;
					map->ledges[i]->flags = (map->ledges[i]->flags + 1) % 2;
				}
			}
		}
	}
}