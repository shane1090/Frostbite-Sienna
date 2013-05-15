#pragma once

#include "Panel.h"
#include "InputManager.h"

class PanelManager
{
public:
	PanelManager(void);
	~PanelManager(void);
	void LoadContent();
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);
	void AddPanel(Panel *&panel);
	bool CheckMouseHover(sf::Vector2<int> mousePos);
	std::vector<Panel*> panels;

private:
	sf::Vector2<int> mousePos, pMousePos;

	int activePanel;
	int draggedPanel;
	int resizingPanel;
};

