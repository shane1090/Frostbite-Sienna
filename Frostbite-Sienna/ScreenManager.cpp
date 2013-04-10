#include "stdafx.h"
#include "ScreenManager.h"

GameScreen *currentScreen, *newScreen;

ScreenManager &ScreenManager::GetInstance()
{
	static ScreenManager instance;
	return instance;
}

ScreenManager::ScreenManager()
{

}

ScreenManager::~ScreenManager()
{

}

void ScreenManager::AddScreen(GameScreen *screen)
{
	currentScreen->UnloadContent();
	delete currentScreen;
	currentScreen = screen;
	currentScreen->LoadContent();
}

void ScreenManager::Initialize()
{
	currentScreen = new EditorScreen();
}

void ScreenManager::LoadContent()
{
	currentScreen->LoadContent();
}

void ScreenManager::Update(sf::RenderWindow &Window)
{
	currentScreen->Update(Window);
}

void ScreenManager::Draw(sf::RenderWindow &window)
{
	currentScreen->Draw(window);
}