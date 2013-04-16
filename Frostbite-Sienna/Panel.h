#pragma once

class Panel
{
public:
	Panel(sf::Rect<float> _position);
	~Panel(void);
	virtual void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	virtual void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);
	sf::Rect<float> position;
	std::string title;
	bool dragged;
	bool resizing;

private:
	sf::Text panelTitle;
	sf::Font font;
};

