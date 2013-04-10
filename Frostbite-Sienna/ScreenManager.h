#pragma once

#include "GameScreen.h"
#include "EditorScreen.h"

class ScreenManager
{
public:
	~ScreenManager();
	static ScreenManager &GetInstance();

	void Initialize();
	void LoadContent();
	void Update(sf::RenderWindow &Window);
	void Draw(sf::RenderWindow &window);

	void AddScreen(GameScreen *screen);

private:
	ScreenManager();
	ScreenManager(ScreenManager const&);
	void operator=(ScreenManager const&);

};