#include "stdafx.h"
#include "PlayTestScreen.h"

PlayTestScreen::PlayTestScreen(void)
{
	scroll = sf::Vector2<float>(-640.0f, -450.0f); // Set 0,0 to center of screen
	showLedges = false;
}

PlayTestScreen::~PlayTestScreen(void)
{

}

void PlayTestScreen::LoadContent()
{
	character = new Character;
	character->map = map;

	if (!font.loadFromFile("Assets/Fonts/arial.ttf"))
		std::cout << "Could not find the specified font" << std::endl;
}

void PlayTestScreen::UnloadContent()
{
	GameScreen::UnloadContent();
}

void PlayTestScreen::SetMapData(Map *map)
{
	this->map = map;
}

void PlayTestScreen::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	InputManager::instance().Poll(Window);

	sf::Time elapsed = gameTime.getElapsedTime();

	float framerate = 1.0f / elapsed.asSeconds();
	sprintf_s(fpsBuffer, "fps: %f", framerate);
	fpsCounter.setString(fpsBuffer);

	scroll += ((character->getLocation() - sf::Vector2f(640.0f, 450.0f)) - scroll) * elapsed.asSeconds() * 20.0f;

	character->Update(Window, gameTime);

	if (InputManager::instance().Pressed(sf::Keyboard::L))
	{
		showLedges = !showLedges;
	}

	// If escape pressed go back to the Editor
	if (InputManager::instance().Pressed(sf::Keyboard::Escape))
	{
		EditorScreen *screen = new EditorScreen;
		screen->SetMapData(map);
		ScreenManager::GetInstance().AddScreen(screen);
	}
}

void PlayTestScreen::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	map->Draw(Window, 0, 2, scroll);
	character->Draw(Window, scroll);
	map->Draw(Window, 3, 5, scroll);

	if (showLedges)
		map->DrawLedges(Window, scroll, -1);

	fpsCounter.setColor(sf::Color(255,255,255,255));
	fpsCounter.setPosition(10, 695);
	fpsCounter.setFont(font);
	fpsCounter.setCharacterSize(14);
	Window.draw(fpsCounter);
}