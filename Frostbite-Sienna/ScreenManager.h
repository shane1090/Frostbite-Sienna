#pragma once

#include "GameScreen.h"
#include "EditorScreen.h"
#include "PlayTestScreen.h"

class ScreenManager
{
public:
	~ScreenManager();
	static ScreenManager &GetInstance();

	void Initialize();
	void LoadContent();
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &window, sf::Clock &gameTime);

	void AddScreen(GameScreen *screen);

private:
	ScreenManager();
	ScreenManager(ScreenManager const&);
	void operator=(ScreenManager const&);

};