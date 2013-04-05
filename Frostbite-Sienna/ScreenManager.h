#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "GameScreen.h"
#include "SplashScreen.h"
#include "TitleScreen.h"
#include "EditorScreen.h"

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