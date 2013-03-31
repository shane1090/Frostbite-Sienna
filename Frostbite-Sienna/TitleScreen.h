#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "GameScreen.h"
#include "ScreenManager.h"

class TitleScreen : public GameScreen
{
public:
	TitleScreen();
	~TitleScreen();

	void LoadContent();
	void UnloadContent();
	void Update(sf::RenderWindow &Window, sf::Event event);
	void Draw(sf::RenderWindow &window);

protected:
private:
	sf::Text text;
	sf::Font font;

};

#endif // TITLESCREEN_H