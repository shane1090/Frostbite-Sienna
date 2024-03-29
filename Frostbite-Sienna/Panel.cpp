#include "stdafx.h"
#include "Panel.h"

Panel::Panel()
{
	this->position = sf::Rect<float>(200,200,200,200);
	this->title = "Test Panel";
	isMoveable = true;
	isResizable = true;
	isCloseable = true;
	this->scrollDrag = false;
	this->minimized = false;
	this->font.loadFromFile("Assets/Fonts/arial.ttf");
	sTargetHeight = 0;
	sMin = 0;
	sMax = 0;
	offset = 0;
}

Panel::~Panel(void)
{

}

void Panel::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	mousePos = InputManager::instance().getMousePosition();

	if (mousePos.x > position.left && mousePos.y > position.top &&
		mousePos.x < (position.left + position.width) && mousePos.y < (position.top + position.height))
	{
		if (sMax > 0.f)
		{
			if ((InputManager::instance().mouseWheel() < 0))
			{
				offset = offset + 30;
			} else if ((InputManager::instance().mouseWheel() > 0))
			{
				offset = offset - 30;
			}
		}
	}

	sf::Rect<float> scrollHandleRect(scrollPos.x, scrollPos.y, 10, scrollHandleHeight - 25);

	if (scrollHandleRect.contains(mousePos.x, mousePos.y))
	{
		if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
			scrollDrag = true;
	}

	if (scrollDrag)
	{
		offset += (mousePos.y - pMousePos.y);

		if (InputManager::instance().Released(sf::Mouse::Button::Left, true))
			scrollDrag = false;
	}

	if (offset < sMin)
		offset = sMin;
	else if (offset > sMax && sMax > 0.f)
		offset = sMax;

	pMousePos = mousePos;
}

void Panel::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw panel background
	sf::RectangleShape segmentShape;
	segmentShape.setPosition(position.left, position.top);
	sf::Vector2<float> segmentSize(position.width, position.height);
	segmentShape.setSize(segmentSize);

	int opacity = 200;
	
	segmentShape.setFillColor(sf::Color(0,0,0,opacity));
	segmentShape.setOutlineColor(sf::Color(255,255,255,opacity));
	segmentShape.setOutlineThickness(1);
	Window.draw(segmentShape);

	// Draw panel header
	sf::RectangleShape headerShape;
	headerShape.setPosition(position.left, position.top);
	sf::Vector2<float> headerSize((position.width), 25);
	headerShape.setSize(headerSize);
	headerShape.setFillColor(sf::Color::Transparent);
	headerShape.setOutlineColor(sf::Color(255,255,255,opacity));
	headerShape.setOutlineThickness(1);
	Window.draw(headerShape);

	if (isCloseable)
	{
		if(mousePos.x > ((position.left + position.width) - 25) && mousePos.x < (position.left + position.width) &&
			mousePos.y > position.top && mousePos.y < (position.top + 25))
		{
			// Draw panel close button
			sf::RectangleShape closeButtonShape;
			closeButtonShape.setPosition((position.left + position.width) - 25, position.top);
			sf::Vector2<float> closeButtonSize((25), 25);
			closeButtonShape.setSize(closeButtonSize);
			closeButtonShape.setFillColor(sf::Color(41,62,96,255));
			Window.draw(closeButtonShape);
		}
	
		sf::Text closeX;
		closeX.setString("x");
		closeX.setFont(font);
		closeX.setCharacterSize(16);
		closeX.setPosition((position.left + position.width) - 16, position.top + 2);
		closeX.setColor(sf::Color::White);
		closeX.setStyle(sf::Text::Bold);
		Window.draw(closeX);
	}

	if (isResizable)
	{
		// Draw panel resize button
		sf::ConvexShape resize;
		resize.setPointCount(3);
		resize.setPoint(0, sf::Vector2f(10, 0));
		resize.setPoint(1, sf::Vector2f(10, 10));
		resize.setPoint(2, sf::Vector2f(0, 10));
		resize.setPosition((position.left + position.width) - 10, (position.top + position.height) - 10);
		resize.setFillColor(sf::Color(255,255,255,opacity));
		Window.draw(resize);
	}

	// Draw panel title
	panelTitle.setString(title);
	panelTitle.setColor(sf::Color(255,255,255,255));
	panelTitle.setPosition((position.left + 10), (position.top + 5));
	panelTitle.setFont(font);
	panelTitle.setCharacterSize(12);
	Window.draw(panelTitle);

	if (sTargetHeight)
	{
		// Draw Scrollbar
		if (sTargetHeight < sPanelHeight)
		{
			scrollbarPercentage = 100.f;
			scrollHandleHeight = sPanelHeight;
		}
		else
		{
			scrollbarPercentage = sPanelHeight / sTargetHeight;
			scrollHandleHeight = scrollbarPercentage * sPanelHeight;
		}

		scrollPos = sf::Vector2<int>((position.left + position.width - 10),
								   position.top + 26 + (scrollbarPercentage * offset));

		if (scrollbarPercentage != 100.f)
		{
			sf::RectangleShape scrollBarShape;
			scrollBarShape.setPosition(scrollPos.x, (position.top + 26));
			sf::Vector2<float> scrollBarSize(10, sPanelHeight);
			scrollBarShape.setSize(scrollBarSize);
			scrollBarShape.setFillColor(sf::Color(46,70,109,200));
			Window.draw(scrollBarShape);
	
			sf::RectangleShape scrollBarHandleShape;
			scrollBarHandleShape.setPosition(scrollPos.x, scrollPos.y);
			sf::Vector2<float> scrollBarHandleSize(10, scrollHandleHeight);
			scrollBarHandleShape.setSize(scrollBarHandleSize);
			scrollBarHandleShape.setFillColor(sf::Color(255,255,255,255));
			Window.draw(scrollBarHandleShape);
		}
	}
}