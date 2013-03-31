#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "GameScreen.h"
#include "ScreenManager.h"

class SplashScreen : public GameScreen
{
public:
	SplashScreen();
	~SplashScreen();

	void LoadContent();
	void UnloadContent();
	void Update(sf::RenderWindow &Window, sf::Event event);
	void Draw(sf::RenderWindow &window);

protected:
private:
	sf::Text text;
	sf::Font font;

};

#endif // SPLASHSCREEN_H