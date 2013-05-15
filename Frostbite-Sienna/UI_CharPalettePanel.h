#pragma once
#include "Panel.h"
#include "CharDef.h"

class UICharPalettePanel : public Panel
{
public:
	UICharPalettePanel(CharDef *&charDef, int &selFrame, std::vector<sf::Texture> &legsTex, std::vector<sf::Texture> &torsoTex, std::vector<sf::Texture> &headTex);
	~UICharPalettePanel(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);
	void DrawPalette(sf::RenderWindow &Window);

private:
	CharDef *charDef;

	std::vector<sf::Texture> &legsTex;
	std::vector<sf::Texture> &torsoTex;
	std::vector<sf::Texture> &headTex;

	int &selFrame;

};

