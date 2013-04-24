#include "stdafx.h"
#include "UI_LayerPanel.h"

UILayerPanel::UILayerPanel(Map *&map, int &curLayer) : curLayer(curLayer), Panel()
{
	this->map = map;
	this->title = "Layers";
	this->position = sf::Rect<float>(1070,50,200,300);
	this->isResizable = false;

	position.height = 45 + (map->layers.size() * 20);
}


UILayerPanel::~UILayerPanel(void)
{

}

void UILayerPanel::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

	if (map->layers.size() > 0)
	{
		// Draw layer info
		for (int i = 0; i < map->layers.size(); i++)
		{
			
			sf::CircleShape circle;
			circle.setRadius(5);
			circle.setOutlineColor(sf::Color::White);
			circle.setOutlineThickness(1);
			circle.setFillColor(sf::Color(94, 103, 113, 255));
			circle.setPosition(position.left + 10, position.top + 35 + (20 * i) + 3);
			Window.draw(circle);

			if (curLayer == i)
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
				if (curLayer != i)
				{
					sf::CircleShape circle;
					circle.setRadius(3);
					circle.setFillColor(sf::Color::White);
					circle.setPosition(position.left + 12, position.top + 35 + (20 * i) + 5);
					Window.draw(circle);
				}

				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					curLayer = i;
				}
			}
			
			text.setString(map->layers[i]->layerName);
			text.setPosition(position.left + 30, position.top + 35 + (20 * i));
			text.setFont(font);
			text.setCharacterSize(12);
			text.setColor(sf::Color::White);
			Window.draw(text);
		}
	}
}