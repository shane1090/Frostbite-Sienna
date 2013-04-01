#include "stdafx.h"
#include "SplashScreen.h"

SplashScreen::SplashScreen()
{

}

SplashScreen::~SplashScreen()
{

}

void SplashScreen::LoadContent()
{
	if (!font.loadFromFile("Assets/Fonts/arial.ttf"))
		std::cout << "Could not find the specified font" << std::endl;

	text.setString("Splash Screen");
	text.setFont(font);

	keys.push_back(sf::Keyboard::Return);
	keys.push_back(sf::Keyboard::Space);
}

void SplashScreen::UnloadContent()
{
	GameScreen::UnloadContent();
}

void SplashScreen::Update(sf::RenderWindow &Window, sf::Event event)
{
	input.Update(Window, event);

	if (input.KeyPressed(keys))
		ScreenManager::GetInstance().AddScreen(new TitleScreen);
}

void SplashScreen::Draw(sf::RenderWindow &Window)
{
	Window.draw(text);
}