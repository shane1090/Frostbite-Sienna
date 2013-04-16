#include "stdafx.h"
#include "Panel.h"

Panel::Panel(sf::Rect<float> position)
{
	this->position = position;
	this->title = "Test Panel";
	font.loadFromFile("Assets/Fonts/arial.ttf");
	dragged = false;
	resizing = false;
}

Panel::~Panel(void)
{

}

void Panel::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{

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

	// Draw panel resize button
	sf::ConvexShape resize;
	resize.setPointCount(3);
	resize.setPoint(0, sf::Vector2f(10, 0));
	resize.setPoint(1, sf::Vector2f(10, 10));
	resize.setPoint(2, sf::Vector2f(0, 10));
	resize.setPosition((position.left + position.width) - 10, (position.top + position.height) - 10);
	resize.setFillColor(sf::Color(255,255,255,opacity));
	Window.draw(resize);

	// Draw panel title
	panelTitle.setString(title);
	panelTitle.setColor(sf::Color(255,255,255,255));
	panelTitle.setPosition((position.left + 10), (position.top + 5));
	panelTitle.setFont(font);
	panelTitle.setCharacterSize(12);
	Window.draw(panelTitle);
}