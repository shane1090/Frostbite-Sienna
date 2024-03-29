#include "stdafx.h"
#include "PanelManager.h"

PanelManager::PanelManager(void)
{
	draggedPanel = -1;
	resizingPanel = -1;
	activePanel = -1;
}

PanelManager::~PanelManager(void)
{
	
}

void PanelManager::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	mousePos = InputManager::instance().getMousePosition();

	for (int i = 0; i < panels.size(); i++)
	{
		if (InputManager::instance().Pressed(sf::Mouse::Button::Left, true))
		{
			if (panels[i]->position.contains(mousePos.x, mousePos.y))
				activePanel = i;

			if (panels[i]->isMoveable)
			{
				// Drag handle
				sf::Rect<float> dragHandle = panels[i]->position;
				dragHandle.height = 25;
				dragHandle.width = dragHandle.width;
				if (dragHandle.contains(mousePos.x, mousePos.y))
				{
					draggedPanel = panels.size() - 1;
				}
			}

			if (panels[i]->isResizable)
			{
				// Resize handle
				sf::Rect<float> resizeHandle = panels[i]->position;
				resizeHandle.left = (resizeHandle.left + resizeHandle.width) - 10;
				resizeHandle.top = (resizeHandle.top + resizeHandle.height) - 10;
				resizeHandle.height = 10;
				resizeHandle.width = 10;
				if (resizeHandle.contains(mousePos.x, mousePos.y))
				{
					resizingPanel = panels.size() - 1;
				}
			}

			if (panels[i]->isCloseable)
			{
				sf::Rect<float> closeHandle = panels[i]->position;
				closeHandle.left = (closeHandle.left + closeHandle.width) - 24;
				closeHandle.top = closeHandle.top + 1;
				closeHandle.height = 23;
				closeHandle.width = 23;
				if (closeHandle.contains(mousePos.x, mousePos.y))
				{
					panels[i]->minimized = true;
				}
			}
		}

		panels[i]->Update(Window,gameTime);
	}

	if (InputManager::instance().Released(sf::Mouse::Button::Left, true))
	{
		draggedPanel = -1;
		resizingPanel = -1;
	}
	else
	{
		if (draggedPanel != -1)
		{
			sf::Rect<float> position = panels[draggedPanel]->position;
			position.left += (mousePos.x - pMousePos.x);
			position.top += (mousePos.y - pMousePos.y);
			panels[draggedPanel]->position = position;
		} else if (resizingPanel != -1)
		{
			sf::Rect<float> position = panels[resizingPanel]->position;
			position.width += (mousePos.x - pMousePos.x);
			position.height += (mousePos.y - pMousePos.y);
			panels[resizingPanel]->position = position;
		}
	}

	if (activePanel != panels.size() && activePanel > -1)
	{
		Panel *p1 = panels[activePanel];
		Panel *p2 = panels[panels.size()-1];

		panels[panels.size()-1] = p1;
		panels[activePanel] = p2;

		activePanel = panels.size() - 1;
	}

	pMousePos = mousePos;
}

void PanelManager::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	for (int i = 0; i < panels.size(); i++)
	{
		if (!panels[i]->minimized)
			panels[i]->Draw(Window, gameTime);
	}
}

void PanelManager::AddPanel(Panel *&panel)
{
	panels.push_back(panel);
}

bool PanelManager::CheckMouseHover(sf::Vector2<int> mousePos)
{
	for (int i = 0; i < panels.size(); i++)
	{
		if (!panels[i]->minimized)
		{
			if (panels[i]->position.contains(mousePos.x, mousePos.y))
				return true;
		}
	}
	return false;
}