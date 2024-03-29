#include "stdafx.h"
#include "ScreenManager.h"

int main()
{
	sf::RenderWindow Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP), "Frostbite Sienna", !sf::Style::Resize | sf::Style::Close);

	ScreenManager::GetInstance().Initialize();
	ScreenManager::GetInstance().LoadContent();

	sf::Clock gameTime;

	while (Window.isOpen())
	{
		Window.clear(sf::Color(100,149,237));

		ScreenManager::GetInstance().Update(Window, gameTime);
		ScreenManager::GetInstance().Draw(Window, gameTime);

		Window.display();

		gameTime.restart();
	}

	return 0;
}