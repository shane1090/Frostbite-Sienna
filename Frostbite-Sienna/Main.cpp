#include "stdafx.h"
#include "ScreenManager.h"
#include "TitleScreen.h"

int main()
{
	sf::RenderWindow Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP), "Frostbite Sienna");

	ScreenManager::GetInstance().Initialize();
	ScreenManager::GetInstance().LoadContent();

	Window.setKeyRepeatEnabled(false);

	while (Window.isOpen())
	{
		sf::Event currentEvent;
		while (Window.pollEvent(currentEvent))
		{
			Window.clear(sf::Color(100,149,237));
		
			if (currentEvent.type == sf::Event::Closed || currentEvent.key.code == sf::Keyboard::Escape)
			{
				Window.close();
			}

			ScreenManager::GetInstance().Update(Window, currentEvent);
			ScreenManager::GetInstance().Draw(Window);

			Window.display();
		}
	}

	return 0;
}