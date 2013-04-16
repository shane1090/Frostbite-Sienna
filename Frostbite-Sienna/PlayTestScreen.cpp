#include "stdafx.h"
#include "PlayTestScreen.h"

PlayTestScreen::PlayTestScreen(void)
{
	scroll = sf::Vector2<float>(-640.0f, -360.0f); // Set 0,0 to center of screen
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

void PlayTestScreen::SetMapData(std::vector<SegmentDefinition*> segDef, std::vector<MapSegment*> mapSeg, std::vector<Ledge*> ledges)
{
	map = new Map;

	map->LoadConfig(segDef, mapSeg, ledges);
}

void PlayTestScreen::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	InputManager::instance().Poll(Window);

	sf::Time elapsed = gameTime.getElapsedTime();

	float framerate = 1.0f / elapsed.asSeconds();
	sprintf(fpsBuffer, "fps: %f", framerate);
	fpsCounter.setString(fpsBuffer);

	scroll += ((character->getLocation() - sf::Vector2f(640.0f, 360.0f)) - scroll) * elapsed.asSeconds() * 20.0f;

	character->Update(Window, gameTime);

	// If escape pressed go back to the Editor
	if (InputManager::instance().Pressed(sf::Keyboard::Escape))
	{
		EditorScreen *screen = new EditorScreen;
		screen->SetMapData(map->segDef, map->mapSeg, map->ledges);
		ScreenManager::GetInstance().AddScreen(screen);
	}
}

void PlayTestScreen::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	map->Draw(Window, 0, 2, scroll);
	character->Draw(Window, scroll);
	map->Draw(Window, 3, 5, scroll);

	fpsCounter.setColor(sf::Color(255,255,255,255));
	fpsCounter.setPosition(10, 695);
	fpsCounter.setFont(font);
	fpsCounter.setCharacterSize(14);
	Window.draw(fpsCounter);
}