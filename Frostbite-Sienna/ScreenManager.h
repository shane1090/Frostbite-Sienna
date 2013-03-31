#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "GameScreen.h"
#include "SplashScreen.h"
#include "TitleScreen.h"
#include "EditorScreen.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;

class ScreenManager
{
public:
	~ScreenManager();
	static ScreenManager &GetInstance();

	void Initialize();
	void LoadContent();
	void Update(sf::RenderWindow &Window, sf::Event event);
	void Draw(sf::RenderWindow &window);

	void AddScreen(GameScreen *screen);
	
protected:
private:
	ScreenManager();
	ScreenManager(ScreenManager const&);
	void operator=(ScreenManager const&);

};

#endif // SCREENMANAGER_H