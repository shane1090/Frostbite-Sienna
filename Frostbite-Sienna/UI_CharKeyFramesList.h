#pragma once
#include "Panel.h"
#include "CharDef.h"

class UICharKeyFramesList : public Panel
{
public:
	UICharKeyFramesList(CharDef *&charDef, int &selAnim, int &selKeyFrame);
	~UICharKeyFramesList(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);

private:
	CharDef *charDef;
	sf::Text text;

	int &selAnim;
	int &selKeyFrame;
};

