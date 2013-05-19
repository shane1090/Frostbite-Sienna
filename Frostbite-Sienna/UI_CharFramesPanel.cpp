#include "stdafx.h"
#include "UI_CharFramesPanel.h"

UICharFramesPanel::UICharFramesPanel(CharDef *&charDef)
{
	this->charDef = charDef;
	this->title = "Frames";
	this->position = sf::Rect<float>(1070,360,200,300);
	this->isResizable = false;
}


UICharFramesPanel::~UICharFramesPanel(void)
{

}

void UICharFramesPanel::Update(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	Panel::Update(Window, gameTime);
}

void UICharFramesPanel::Draw(sf::RenderWindow &Window, sf::Clock &gameTime)
{
	// Draw the panel first
	Panel::Draw(Window, gameTime);

}