#pragma once
#include "Panel.h"
#include "CharDef.h"

class UICharFramesPanel : public Panel
{
public:
	UICharFramesPanel(CharDef *&charDef, int &selPart, int &selFrame, int &selAnim);
	~UICharFramesPanel(void);
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);

private:
	void EditFrameName();
	void CopyFrame(int src, int dest);

	CharDef *charDef;
	sf::Text text;

	int &selPart;
	int &selFrame;
	int &selAnim;

	bool editingFrameName;
};

