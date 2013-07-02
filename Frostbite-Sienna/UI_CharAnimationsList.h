#pragma once
#include "Panel.h"
#include "CharDef.h"

class UICharAnimationsList : public Panel
{
public:
	UICharAnimationsList(CharDef *&charDef, int &selPart, int &selFrame, int &selAnim);
	~UICharAnimationsList(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);

private:
	void EditName();

	CharDef *charDef;
	sf::Text text;

	int &selPart;
	int &selFrame;
	int &selAnim;

	bool editingName;
};

