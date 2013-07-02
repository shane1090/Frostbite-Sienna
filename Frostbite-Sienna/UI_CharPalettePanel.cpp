#include "stdafx.h"
#include "UI_CharPalettePanel.h"

UICharPalettePanel::UICharPalettePanel(CharDef *&charDef, int &selFrame, std::vector<sf::Texture> &legsTex, std::vector<sf::Texture> &torsoTex, std::vector<sf::Texture> &headTex) 
	: selFrame(selFrame), legsTex(legsTex), torsoTex(torsoTex), headTex(headTex), Panel()
{
	this->charDef = charDef;
	this->position = sf::Rect<float>(10,510,500,200);
	this->title = "Character Palette";
	this->isResizable = false;
	this->isCloseable = false;
	this->selFrame = selFrame;
	this->legsTex = legsTex;
	this->headTex = headTex;
	this->torsoTex = torsoTex;
}


UICharPalettePanel::~UICharPalettePanel(void)
{

}

void UICharPalettePanel::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	Panel::Update(Window, gameTime);
}

void UICharPalettePanel::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

	DrawPalette(Window);
}

void UICharPalettePanel::DrawPalette(sf::RenderWindow &Window)
{
	for (int l = 0; l < 3; l++)
	{
		sf::Sprite sprite;

		switch (l)
		{
			case 0:
				sprite.setTexture(headTex[charDef->headIndex]);
				break;
			case 1:
				sprite.setTexture(torsoTex[charDef->torsoIndex]);
				break;
			case 2:
				sprite.setTexture(legsTex[charDef->legsIndex]);
				break;
		}

		for (int i = 0; i < 25; i++)
		{
			sf::Rect<int> sRect;
			sRect.left = (i % 5) * 64;
			sRect.top = (i / 5) * 64;
			sRect.width = 64;
			sRect.height = 64;

			// draw
			sprite.setTextureRect(sRect);
			sprite.setScale(.5f, .5f);
			sf::Vector2i itemPos(position.left + 10 + (i * 32), position.top + 10 + (26 + (l * 32)));
			sprite.setPosition(itemPos.x, itemPos.y);


			if (mousePos.x > itemPos.x && mousePos.x < (itemPos.x + 32) &&
				mousePos.y > itemPos.y && mousePos.y < (itemPos.y + 32))
			{
				// Draw panel background
				sf::RectangleShape hoverRect;
				hoverRect.setPosition(itemPos.x, itemPos.y);
				hoverRect.setSize(sf::Vector2f(32.0f, 32.0f));
				hoverRect.setFillColor(sf::Color(255,255,255,255));
				Window.draw(hoverRect);
				
				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					charDef->frames[selFrame]->parts.push_back(new Part(i + 64 * l));
					std::cout << "Added new part: " << (i + 64 * l) << std::endl;
				}
			}

			Window.draw(sprite);
		}
	}
}