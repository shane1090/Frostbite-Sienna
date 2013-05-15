#pragma once
#include "Panel.h"
#include "CharDef.h"

class UICharPartsList : public Panel
{
public:
	UICharPartsList(CharDef *&charDef, int &selFrame, int &selPart);
	~UICharPartsList(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);
	void SwapParts(int idx1, int idx2);

private:
	CharDef *charDef;
	sf::Text text;

	int &selFrame;
	int &selPart;

};

