#include "stdafx.h"
#include "PanelManager.h"

PanelManager::PanelManager(void)
{

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
			// Drag handle
			sf::Rect<float> dragHandle = panels[i]->position;
			dragHandle.height = 25;
			dragHandle.width = dragHandle.width;
			if (dragHandle.contains(mousePos.x, mousePos.y))
			{
				panels[i]->dragged = true;
			}

			// Resize handle
			sf::Rect<float> resizeHandle = panels[i]->position;
			resizeHandle.left = (resizeHandle.left + resizeHandle.width) - 10;
			resizeHandle.top = (resizeHandle.top + resizeHandle.height) - 10;
			resizeHandle.height = 10;
			resizeHandle.width = 10;
			if (resizeHandle.contains(mousePos.x, mousePos.y))
			{
				panels[i]->resizing = true;
			}
		}

		if (InputManager::instance().Released(sf::Mouse::Button::Left, true))
		{
			panels[i]->dragged = false;
			panels[i]->resizing = false;
		}
		else
		{
			sf::Rect<float> position = panels[i]->position;

			if (panels[i]->dragged)
			{
				position.left += (mousePos.x - pMousePos.x);
				position.top += (mousePos.y - pMousePos.y);
				panels[i]->position = position;
			} else if (panels[i]->resizing)
			{
				position.width += (mousePos.x - pMousePos.x);
				position.height += (mousePos.y - pMousePos.y);
				panels[i]->position = position;
			}
		}

		panels[i]->Update(Window,gameTime);
	}

	pMousePos = mousePos;
}

void PanelManager::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	for (int i = 0; i < panels.size(); i++)
	{
		panels[i]->Draw(Window, gameTime);
	}
}

void PanelManager::AddPanel(Panel *&panel)
{
	panels.push_back(panel);
}