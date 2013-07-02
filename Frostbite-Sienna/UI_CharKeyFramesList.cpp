#include "stdafx.h"
#include "UI_CharKeyFramesList.h"

UICharKeyFramesList::UICharKeyFramesList(CharDef *&charDef, int &selAnim, int &selKeyFrame) : selAnim(selAnim), selKeyFrame(selKeyFrame), Panel()
{
	this->charDef = charDef;
	this->title = "Key Frames";
	this->position = sf::Rect<float>(10,260,200,200);
	this->isResizable = false;
	this->isCloseable = false;

	// Set scrolling min and max heights
	sPanelHeight = 200 - 26;
}


UICharKeyFramesList::~UICharKeyFramesList(void)
{

}

void UICharKeyFramesList::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	Panel::Update(Window, gameTime);

	// Update target and max height for scrolling
	sTargetHeight = ceil((float)charDef->animations[selAnim]->keyFrames.size() * 22.f);
	sMax = ceil((float)charDef->animations[selAnim]->keyFrames.size() * 22.f) - sPanelHeight;

	sf::FloatRect panelRect((position.left) / 1280.f,
							(position.top + 26) / 720.f,
							(position.width) / 1280.f,
							(position.height - 26) / 720.f);

	panelView.reset(sf::FloatRect(
		(position.left),
		(position.top + 26),
		(position.width),
		(position.height - 26)));
	
	panelView.setViewport(panelRect);
}

void UICharKeyFramesList::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

	text.setFont(font);
	text.setCharacterSize(12);

	Window.setView(panelView);
	
	for (int i = 0; i < charDef->animations[selAnim]->keyFrames.size(); i++)
	{
		text.setColor(sf::Color::White);

		sf::Vector2f itemPos(position.left, position.top + 31 + (22 * i) - offset);
		
		sf::CircleShape circle;
		circle.setRadius(5);
		circle.setOutlineColor(sf::Color::White);
		circle.setOutlineThickness(1);
		circle.setFillColor(sf::Color(94, 103, 113, 255));
		circle.setPosition(itemPos.x + 10, itemPos.y + 6);
		Window.draw(circle);

		int frameRef = charDef->animations[selAnim]->keyFrames[i]->frameRef;

		std::string keyFrameName = charDef->frames[frameRef]->name;

		text.setString(keyFrameName);
		text.setPosition(itemPos.x + 30, itemPos.y + 3);
		Window.draw(text);

		if (selKeyFrame == i)
		{
			sf::CircleShape circle;
			circle.setRadius(3);
			circle.setFillColor(sf::Color::White);
			circle.setPosition(itemPos.x + 12, itemPos.y + 8);
			Window.draw(circle);
		}

		// This should be moved to Update
		if (mousePos.x > itemPos.x && mousePos.x < itemPos.x + 100 &&
			mousePos.y > itemPos.y && mousePos.y < itemPos.y + 22)
		{
			if (selKeyFrame != i)
			{
				sf::CircleShape circle;
				circle.setRadius(3);
				circle.setFillColor(sf::Color::White);
				circle.setPosition(itemPos.x + 12, itemPos.y + 8);
				Window.draw(circle);

				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					selKeyFrame = i;
				}
			}
		}

		text.setString("-");
		text.setColor(sf::Color::White);
		text.setPosition(itemPos.x + 110, itemPos.y + 3);
		if (mousePos.x > itemPos.x + 110 && mousePos.x < itemPos.x + 120 &&
			mousePos.y > itemPos.y && mousePos.y < itemPos.y + 22)
		{
			text.setColor(sf::Color::Red);

			if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
			{
				charDef->animations[selAnim]->keyFrames[i]->duration--;
			}
		}
		Window.draw(text);

		text.setString(Convert(charDef->animations[selAnim]->keyFrames[i]->duration));
		text.setColor(sf::Color::White);
		text.setPosition(itemPos.x + 120, itemPos.y + 3);

		sf::FloatRect thisStringSize = text.getLocalBounds();

		Window.draw(text);

		text.setString("+");
		text.setColor(sf::Color::White);
		text.setPosition(itemPos.x + 125 + thisStringSize.width, itemPos.y + 3);
		if (mousePos.x > itemPos.x + 125 + thisStringSize.width && mousePos.x < itemPos.x + 135 + thisStringSize.width &&
			mousePos.y > itemPos.y && mousePos.y < itemPos.y + 22)
		{
			text.setColor(sf::Color::Red);

			if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
			{
				charDef->animations[selAnim]->keyFrames[i]->duration++;
			}
		}
		Window.draw(text);
	}

	Window.setView(Window.getDefaultView());
}