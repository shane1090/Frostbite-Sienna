#include "stdafx.h"
#include "UI_CharAnimationsList.h"

UICharAnimationsList::UICharAnimationsList(CharDef *&charDef, int &selPart, int &selFrame, int &selAnim) : selPart(selPart), selFrame(selFrame), selAnim(selAnim), Panel()
{
	this->charDef = charDef;
	this->title = "Animations";
	this->position = sf::Rect<float>(10,50,200,200);
	this->isResizable = false;
	this->isCloseable = false;
	this->editingName = false;

	// Set scrolling min and max heights
	sPanelHeight = 200 - 55;
}


UICharAnimationsList::~UICharAnimationsList(void)
{

}

void UICharAnimationsList::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	Panel::Update(Window, gameTime);

	// Update target and max height for scrolling
	sTargetHeight = ceil((float)charDef->animations.size() * 22.f);
	sMax = ceil((float)charDef->animations.size() * 22.f) - sPanelHeight;

	if(editingName)
		EditName();

	sf::FloatRect panelRect((position.left) / 1280.f,
							(position.top + 26) / 720.f,
							(position.width) / 1280.f,
							(position.height - 55) / 720.f);

	panelView.reset(sf::FloatRect(
		(position.left),
		(position.top + 26),
		(position.width),
		(position.height - 55)));
	
	panelView.setViewport(panelRect);
}

void UICharAnimationsList::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

	text.setFont(font);
	text.setCharacterSize(12);

	Window.setView(panelView);
	
	for (int i = 0; i < charDef->animations.size(); i++)
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

		std::string animationName = charDef->animations[i]->name;

		if (editingName && selAnim == i)
			animationName += "*";

		text.setString(animationName);
		text.setPosition(itemPos.x + 30, itemPos.y + 3);
		Window.draw(text);

		if (selAnim == i)
		{
			sf::CircleShape circle;
			circle.setRadius(3);
			circle.setFillColor(sf::Color::White);
			circle.setPosition(itemPos.x + 12, itemPos.y + 8);
			Window.draw(circle);

			if (mousePos.x > itemPos.x && mousePos.x < itemPos.x + 100 &&
			mousePos.y > itemPos.y && mousePos.y < itemPos.y + 22)
			{
				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					editingName = true;
				}
			}
		}

		// This should be moved to Update
		if (mousePos.x > itemPos.x && mousePos.x < itemPos.x + 100 &&
			mousePos.y > itemPos.y && mousePos.y < itemPos.y + 22)
		{
			if (selAnim != i)
			{
				sf::CircleShape circle;
				circle.setRadius(3);
				circle.setFillColor(sf::Color::White);
				circle.setPosition(itemPos.x + 12, itemPos.y + 8);
				Window.draw(circle);

				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					selAnim = i;
					selPart = -1;
					editingName = false;
				}
			}
		}
	}

	Window.setView(Window.getDefaultView());
	
	text.setColor(sf::Color::White);
	text.setString("Add animation");
	text.setPosition(position.left + 10, (position.top + position.height) - 20);
	if (mousePos.x > position.left && mousePos.x < position.left + 100 &&
		mousePos.y > (position.top + position.height) - 20 && mousePos.y < (position.top + position.height) - 1)
	{
		text.setColor(sf::Color::Red);
		if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
		{
			charDef->animations.push_back(new Animation);
		}
	}
	
	Window.draw(text);
}

void UICharAnimationsList::EditName()
{
	std::string t = "";
	t = charDef->animations[selAnim]->name;

	if (InputManager::instance().Pressed(sf::Keyboard::BackSpace))
	{
		if (t.length() > 0) 
			t = t.substr(0, t.length() - 1);
	}
	else if (InputManager::instance().Pressed(sf::Keyboard::Return))
	{
		editingName = false;
	}
	else
	{
		char n = InputManager::instance().GetCurrentKey();
		if (n < 128 && n >= 32)
			t += n;
	}

	charDef->animations[selAnim]->name = t;
}