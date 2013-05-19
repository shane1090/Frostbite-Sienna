#pragma once
#include "Panel.h"
#include "CharDef.h"

class UICharFramesPanel : public Panel
{
public:
	UICharFramesPanel(CharDef *&charDef);
	~UICharFramesPanel(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);

private:
	CharDef *charDef;

};

