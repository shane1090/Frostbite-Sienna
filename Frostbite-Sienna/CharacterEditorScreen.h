#pragma once

#include "GameScreen.h"
#include "ScreenManager.h"
#include "CharDef.h"
#include "PanelManager.h"
#include "UI_CharPalettePanel.h"

class CharacterEditorScreen : public GameScreen
{
public:
	CharacterEditorScreen(void);
	~CharacterEditorScreen(void);
	void LoadContent();
	void UnloadContent();
	void Update(sf::RenderWindow &Window, sf::Clock &gameTime);
	void Draw(sf::RenderWindow &Window, sf::Clock &gameTime);
	void DrawCharacter(sf::Vector2f loc, float scale, int face, int frameIndex, bool preview, float alpha, sf::RenderWindow &Window);
	void DrawToolBar(sf::RenderWindow &Window);
	bool DrawButton(sf::RenderWindow &Window, int x, int y, int index);
	void DrawPalette(sf::RenderWindow &Window);
	void DrawPartsList(sf::RenderWindow &Window);

private:
	void LoadTextures(std::vector<sf::Texture> &textures, std::string path);

	CharDef *charDef;

	int FACE_LEFT;
	int FACE_RIGHT;

	std::vector<sf::Texture> legsTex;
	std::vector<sf::Texture> torsoTex;
	std::vector<sf::Texture> headTex;

	int selPart, selFrame;

	sf::Texture toolbarIconsTex;
	sf::Font font;

	sf::Vector2<int> mousePos, pMousePos;

	sf::Text text;

	PanelManager *panelManager;
	Panel *charPalettePane;
};

