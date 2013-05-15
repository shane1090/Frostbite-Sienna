#include "stdafx.h"
#include "UI_CharPartsList.h"

UICharPartsList::UICharPartsList(CharDef *&charDef, int &selFrame, int &selPart) : selFrame(selFrame), selPart(selPart), Panel()
{
	this->charDef = charDef;
	this->title = "Parts List";
	this->position = sf::Rect<float>(1070,50,200,300);
	this->isResizable = false;
}

UICharPartsList::~UICharPartsList(void)
{

}

void UICharPartsList::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	Panel::Update(Window, gameTime);
}

void UICharPartsList::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

	text.setFont(font);
	text.setCharacterSize(12);
	text.setColor(sf::Color::White);
	
	for (int i = 0; i < charDef->frames[selFrame]->parts.size(); i++)
	{
		int y = position.top + 35 + (20 * i);
		int index = charDef->frames[selFrame]->parts[i]->index;
		
		sf::CircleShape circle;
		circle.setRadius(5);
		circle.setOutlineColor(sf::Color::White);
		circle.setOutlineThickness(1);
		circle.setFillColor(sf::Color(94, 103, 113, 255));
		circle.setPosition(position.left + 10, y + 3);
		Window.draw(circle);

		if (selPart == i)
		{
			sf::CircleShape circle;
			circle.setRadius(3);
			circle.setFillColor(sf::Color::White);
			circle.setPosition(position.left + 12, y + 5);
			Window.draw(circle);
		}
		
		if (index < 64)
			text.setString("head " + Convert(index));
		else if (index < 74)
			text.setString("torso " + Convert(index));
		else if (index < 128)
			text.setString("arms " + Convert(index));
		else
			text.setString("legs " + Convert(index));

		text.setPosition(position.left + 30, y);
		
		Window.draw(text);

		// This should be moved to Update
		if (mousePos.x > position.left && mousePos.x < position.left + 100 &&
			mousePos.y > y && mousePos.y < y + 14)
		{
			if (selPart != i)
			{
				sf::CircleShape circle;
				circle.setRadius(3);
				circle.setFillColor(sf::Color::White);
				circle.setPosition(position.left + 12, position.top + 35 + (20 * i) + 5);
				Window.draw(circle);
			}

			if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
			{
				selPart = i;
			}
		}

		// Draw down arrow button
		sf::ConvexShape upArrow;
		upArrow.setPointCount(3);
		upArrow.setPoint(0, sf::Vector2f(5, 0));
		upArrow.setPoint(1, sf::Vector2f(10, 10));
		upArrow.setPoint(2, sf::Vector2f(0, 10));
		upArrow.setPosition(position.left + 120, y + 1);
		upArrow.setFillColor(sf::Color(255,255,255,255));
		

		sf::ConvexShape downArrow;
		downArrow.setPointCount(3);
		downArrow.setPoint(0, sf::Vector2f(0, 0));
		downArrow.setPoint(1, sf::Vector2f(10, 0));
		downArrow.setPoint(2, sf::Vector2f(5, 10));
		downArrow.setPosition(position.left + 135, y);
		downArrow.setFillColor(sf::Color(255,255,255,255));
		

		if (mousePos.x > position.left + 120 && mousePos.x < position.left + 130 &&
			mousePos.y > y + 1 && mousePos.y < y + 14)
		{
			upArrow.setFillColor(sf::Color::Red);

			if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
			{
				SwapParts(i, i - 1);
			}
		}

		if (mousePos.x > position.left + 135 && mousePos.x < position.left + 145 &&
			mousePos.y > y && mousePos.y < y + 14)
		{
			downArrow.setFillColor(sf::Color::Red);

			if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
			{
				SwapParts(i, i + 1);
			}
		}

		Window.draw(upArrow);
		Window.draw(downArrow);
	}
}

void UICharPartsList::SwapParts(int idx1, int idx2)
{
	if (idx1 < 0 || idx2 < 0 ||
		idx1 >= charDef->frames[selFrame]->parts.size() ||
		idx2 >= charDef->frames[selFrame]->parts.size())
		return;

	Part *i = charDef->frames[selFrame]->parts[idx1];
	Part *j = charDef->frames[selFrame]->parts[idx2];

	charDef->frames[selFrame]->parts[idx1] = j;
	charDef->frames[selFrame]->parts[idx2] = i;
}
