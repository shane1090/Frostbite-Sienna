#include "stdafx.h"
#include "UI_CharFramesPanel.h"

UICharFramesPanel::UICharFramesPanel(CharDef *&charDef, int &selPart, int &selFrame, int &selAnim) : selPart(selPart), selFrame(selFrame), selAnim(selAnim), Panel()
{
	this->charDef = charDef;
	this->title = "Frames";
	this->position = sf::Rect<float>(1070,360,200,300);
	this->isResizable = false;
	this->isCloseable = false;
	this->editingFrameName = false;

	// Set scrolling min and max heights
	sPanelHeight = 300 - 55;
}


UICharFramesPanel::~UICharFramesPanel(void)
{

}

void UICharFramesPanel::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	Panel::Update(Window, gameTime);

	// Update target and max height for scrolling
	sTargetHeight = ceil((float)charDef->frames.size() * 22.f);
	sMax = ceil((float)charDef->frames.size() * 22.f) - sPanelHeight;

	if(editingFrameName)
		EditFrameName();

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

void UICharFramesPanel::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

	text.setFont(font);
	text.setCharacterSize(12);

	Window.setView(panelView);
	
	for (int i = 0; i < charDef->frames.size(); i++)
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

		std::string frameName = charDef->frames[i]->name;

		if (editingFrameName && selFrame == i)
			frameName += "*";

		text.setString(frameName);
		text.setPosition(itemPos.x + 30, itemPos.y + 3);
		Window.draw(text);

		if (selFrame == i)
		{
			sf::CircleShape circle;
			circle.setRadius(3);
			circle.setFillColor(sf::Color::White);
			circle.setPosition(itemPos.x + 12, itemPos.y + 8);
			Window.draw(circle);

			text.setString("(a)");
			text.setPosition(itemPos.x + 150, itemPos.y + 3);

			if (mousePos.x > itemPos.x + 150 && mousePos.x < itemPos.x + 170 &&
				mousePos.y > itemPos.y && mousePos.y < itemPos.y + 22)
			{
				text.setColor(sf::Color::Red);

				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					charDef->animations[selAnim]->keyFrames.push_back(new KeyFrame(i, 1));
				}
			}
			Window.draw(text);

			if (mousePos.x > itemPos.x && mousePos.x < itemPos.x + 100 &&
			mousePos.y > itemPos.y && mousePos.y < itemPos.y + 22)
			{
				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					editingFrameName = true;
				}
			}
		}

		// This should be moved to Update
		if (mousePos.x > itemPos.x && mousePos.x < itemPos.x + 100 &&
			mousePos.y > itemPos.y && mousePos.y < itemPos.y + 22)
		{
			if (selFrame != i)
			{
				sf::CircleShape circle;
				circle.setRadius(3);
				circle.setFillColor(sf::Color::White);
				circle.setPosition(itemPos.x + 12, itemPos.y + 8);
				Window.draw(circle);

				if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
				{
					selFrame = i;
					selPart = -1;
					editingFrameName = false;
				}
			}
		}
	}

	Window.setView(Window.getDefaultView());
	
	text.setColor(sf::Color::White);
	text.setString("Add frame");
	text.setPosition(position.left + 10, (position.top + position.height) - 20);
	if (mousePos.x > position.left && mousePos.x < position.left + 100 &&
		mousePos.y > (position.top + position.height) - 20 && mousePos.y < (position.top + position.height) - 1)
	{
		text.setColor(sf::Color::Red);
		if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
		{
			charDef->frames.push_back(new Frame);
			CopyFrame(selFrame, charDef->frames.size() - 1);
		}
	}
	
	Window.draw(text);
}

void UICharFramesPanel::EditFrameName()
{
	std::string t = "";
	t = charDef->frames[selFrame]->name;

	if (InputManager::instance().Pressed(sf::Keyboard::BackSpace))
	{
		if (t.length() > 0) 
			t = t.substr(0, t.length() - 1);
	}
	else if (InputManager::instance().Pressed(sf::Keyboard::Return))
	{
		editingFrameName = false;
	}
	else
	{
		char n = InputManager::instance().GetCurrentKey();

		if (n < 128 && n >= 32)
			t += n;
	}

	charDef->frames[selFrame]->name = t;
}

void UICharFramesPanel::CopyFrame(int src, int dest)
{
	Frame *keySrc = charDef->frames[src];

	for (int i = 0; i < keySrc->parts.size(); i++)
	{
		Part *srcPart = keySrc->parts[i];
		Part *destPart = new Part(srcPart->index);
		destPart->location = srcPart->location;
		destPart->rotation = srcPart->rotation;
		destPart->scaling = srcPart->scaling;

		charDef->frames[dest]->parts.push_back(destPart);
	}
}