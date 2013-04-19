#pragma once

#include "InputManager.h"

class Panel
{
public:
	Panel(void);
	~Panel(void);
	virtual void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	virtual void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);
	sf::Rect<float> position;
	std::string title;
	bool dragged;
	bool resizing;
	bool isMoveable;
	bool isResizable;
	bool minimized;

	sf::View panelView;

	int offset;
	int sMin, sMax;
	float sPanelHeight, sTargetHeight;

protected:
	sf::Vector2<int> mousePos, pMousePos;

private:
	sf::Text panelTitle;
	sf::Font font;

	float scrollbarPercentage;
	float scrollHandleHeight;

	sf::Vector2<int> scrollPos;

	bool scrollDrag;
};

