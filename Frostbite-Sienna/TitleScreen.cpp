#include "stdafx.h"
#include "TitleScreen.h"

TitleScreen::TitleScreen()
{

}

TitleScreen::~TitleScreen()
{

}

void TitleScreen::LoadContent()
{
	if (!font.loadFromFile("Assets/Fonts/arial.ttf"))
		std::cout << "Could not find the specified font" << std::endl;

	text.setString("Title Screen");
	text.setFont(font);

	keys.push_back(sf::Keyboard::Return);
	keys.push_back(sf::Keyboard::Space);
}

void TitleScreen::UnloadContent()
{
	GameScreen::UnloadContent();
}

void TitleScreen::Update(sf::RenderWindow &Window, sf::Event event)
{
	input.Update(event);

	if (input.KeyPressed(keys))
		ScreenManager::GetInstance().AddScreen(new EditorScreen);
}

void TitleScreen::Draw(sf::RenderWindow &Window)
{
	Window.draw(text);
}