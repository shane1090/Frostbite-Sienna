#pragma once
#include "Panel.h"
#include "CharDef.h"

class UICharPalettePanel : public Panel
{
public:
	UICharPalettePanel(CharDef *&charDef);
	~UICharPalettePanel(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);

private:
	CharDef *charDef;

};

